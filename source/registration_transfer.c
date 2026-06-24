#include "registration_transfer.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REG_LINE_BUFFER 512

static void set_error(char *buffer, size_t size, const char *message) {
  if (buffer != NULL && size > 0) {
    snprintf(buffer, size, "%s", message);
  }
}

static int ensure_capacity(
    registration_eos_table *table,
    size_t *capacity,
    size_t requested,
    char *error_message,
    size_t error_message_size) {
  if (requested <= *capacity) return 0;
  size_t new_capacity = (*capacity == 0) ? 64 : 2 * (*capacity);
  while (new_capacity < requested) new_capacity *= 2;

  double *new_log_temperature = realloc(table->log_temperature, new_capacity * sizeof(double));
  if (new_log_temperature == NULL) {
    set_error(error_message, error_message_size, "allocation failure for temperature array");
    return 1;
  }
  table->log_temperature = new_log_temperature;

  double *new_g_rho = realloc(table->g_rho, new_capacity * sizeof(double));
  if (new_g_rho == NULL) {
    set_error(error_message, error_message_size, "allocation failure for g_rho array");
    return 1;
  }
  table->g_rho = new_g_rho;

  double *new_g_p = realloc(table->g_p, new_capacity * sizeof(double));
  if (new_g_p == NULL) {
    set_error(error_message, error_message_size, "allocation failure for g_p array");
    return 1;
  }
  table->g_p = new_g_p;

  double *new_g_s = realloc(table->g_s, new_capacity * sizeof(double));
  if (new_g_s == NULL) {
    set_error(error_message, error_message_size, "allocation failure for g_s array");
    return 1;
  }
  table->g_s = new_g_s;

  *capacity = new_capacity;
  return 0;
}

void registration_eos_free(registration_eos_table *table) {
  if (table == NULL) return;
  free(table->log_temperature);
  free(table->g_rho);
  free(table->g_p);
  free(table->g_s);
  memset(table, 0, sizeof(*table));
}

int registration_eos_load_csv(
    const char *path,
    registration_eos_table *table,
    char *error_message,
    size_t error_message_size) {
  if (path == NULL || table == NULL) {
    set_error(error_message, error_message_size, "null path or table");
    return 1;
  }

  memset(table, 0, sizeof(*table));
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    char message[REG_LINE_BUFFER];
    snprintf(message, sizeof(message), "cannot open EOS table '%s': %s", path, strerror(errno));
    set_error(error_message, error_message_size, message);
    return 1;
  }

  size_t capacity = 0;
  char line[REG_LINE_BUFFER];
  while (fgets(line, sizeof(line), file) != NULL) {
    char *cursor = line;
    while (*cursor == ' ' || *cursor == '\t') cursor++;
    if (*cursor == '#' || *cursor == '\n' || *cursor == '\0') continue;
    if (strstr(cursor, "T_GeV") != NULL) continue;

    double temperature = 0.0, g_rho = 0.0, g_p = 0.0, g_s = 0.0;
    if (sscanf(cursor, " %lf , %lf , %lf , %lf", &temperature, &g_rho, &g_p, &g_s) != 4) {
      fclose(file);
      registration_eos_free(table);
      set_error(error_message, error_message_size, "malformed EOS CSV row");
      return 1;
    }
    if (!(temperature > 0.0 && g_rho > 0.0 && g_p > 0.0 && g_s > 0.0)) {
      fclose(file);
      registration_eos_free(table);
      set_error(error_message, error_message_size, "EOS table values must be positive");
      return 1;
    }
    if (table->size > 0 && log(temperature) <= table->log_temperature[table->size - 1]) {
      fclose(file);
      registration_eos_free(table);
      set_error(error_message, error_message_size, "EOS temperatures must be strictly increasing");
      return 1;
    }
    if (ensure_capacity(table, &capacity, table->size + 1, error_message, error_message_size)) {
      fclose(file);
      registration_eos_free(table);
      return 1;
    }
    table->log_temperature[table->size] = log(temperature);
    table->g_rho[table->size] = g_rho;
    table->g_p[table->size] = g_p;
    table->g_s[table->size] = g_s;
    table->size++;
  }
  fclose(file);

  if (table->size < 2) {
    registration_eos_free(table);
    set_error(error_message, error_message_size, "EOS table requires at least two data rows");
    return 1;
  }
  return 0;
}

static size_t lower_interval(const registration_eos_table *table, double log_temperature) {
  size_t lo = 0;
  size_t hi = table->size - 1;
  while (hi - lo > 1) {
    size_t mid = lo + (hi - lo) / 2;
    if (table->log_temperature[mid] <= log_temperature) lo = mid;
    else hi = mid;
  }
  return lo;
}

static int interpolate_all(
    const registration_eos_table *table,
    double log_temperature,
    double *g_rho,
    double *g_p,
    double *g_s,
    double *dln_g_rho_dln_t,
    double *dln_g_p_dln_t,
    double *dln_g_s_dln_t) {
  if (log_temperature < table->log_temperature[0] ||
      log_temperature > table->log_temperature[table->size - 1]) return 1;

  if (log_temperature == table->log_temperature[table->size - 1]) {
    log_temperature = nextafter(log_temperature, -INFINITY);
  }
  size_t i = lower_interval(table, log_temperature);
  double x0 = table->log_temperature[i];
  double x1 = table->log_temperature[i + 1];
  double f = (log_temperature - x0) / (x1 - x0);

  *g_rho = exp((1.0 - f) * log(table->g_rho[i]) + f * log(table->g_rho[i + 1]));
  *g_p = exp((1.0 - f) * log(table->g_p[i]) + f * log(table->g_p[i + 1]));
  *g_s = exp((1.0 - f) * log(table->g_s[i]) + f * log(table->g_s[i + 1]));
  *dln_g_rho_dln_t = (log(table->g_rho[i + 1]) - log(table->g_rho[i])) / (x1 - x0);
  *dln_g_p_dln_t = (log(table->g_p[i + 1]) - log(table->g_p[i])) / (x1 - x0);
  *dln_g_s_dln_t = (log(table->g_s[i + 1]) - log(table->g_s[i])) / (x1 - x0);
  return 0;
}

int registration_eos_evaluate(
    const registration_eos_table *table,
    double temperature_gev,
    double trace_power,
    double trace_floor,
    registration_thermo_state *state,
    char *error_message,
    size_t error_message_size) {
  if (table == NULL || state == NULL || table->size < 2) {
    set_error(error_message, error_message_size, "uninitialized EOS table or null state");
    return 1;
  }
  if (!(temperature_gev > 0.0 && trace_power > 0.0 && trace_floor > 0.0)) {
    set_error(error_message, error_message_size, "temperature, trace power, and trace floor must be positive");
    return 1;
  }

  double dln_gr = 0.0, dln_gp = 0.0, dln_gs = 0.0;
  if (interpolate_all(table, log(temperature_gev), &state->g_rho, &state->g_p,
                      &state->g_s, &dln_gr, &dln_gp, &dln_gs)) {
    set_error(error_message, error_message_size, "temperature lies outside EOS table range");
    return 1;
  }

  state->dln_g_rho_dln_temperature = dln_gr;
  state->dln_g_p_dln_temperature = dln_gp;
  state->dln_g_s_dln_temperature = dln_gs;

  double ratio = state->g_p / state->g_rho;
  double raw_trace = fabs(1.0 - ratio);
  state->trace_fraction = fmax(raw_trace, trace_floor);
  state->activation = pow(state->trace_fraction, trace_power);

  if (raw_trace <= trace_floor) {
    state->dln_activation_dln_temperature = 0.0;
  } else {
    double sign = (1.0 - ratio >= 0.0) ? 1.0 : -1.0;
    double dtrace_dln_t = -sign * ratio * (dln_gp - dln_gr);
    state->dln_activation_dln_temperature =
        trace_power * dtrace_dln_t / state->trace_fraction;
  }
  return 0;
}

int registration_transfer_evaluate(
    const registration_eos_table *table,
    const registration_transfer_parameters *parameters,
    double temperature_gev,
    double hubble_rate,
    double rho_flux,
    double delta_theta_over_theta,
    double delta_rho_flux_over_rho_flux,
    double delta_temperature_over_temperature,
    registration_transfer_state *state,
    char *error_message,
    size_t error_message_size) {
  if (parameters == NULL || state == NULL) {
    set_error(error_message, error_message_size, "null transfer parameters or state");
    return 1;
  }
  if (!(parameters->lambda0 >= 0.0 && hubble_rate >= 0.0 && rho_flux >= 0.0)) {
    set_error(error_message, error_message_size, "lambda0, H, and rho_flux must be non-negative");
    return 1;
  }
  if (registration_eos_evaluate(table, temperature_gev, parameters->trace_power,
                                parameters->trace_floor, &state->thermo,
                                error_message, error_message_size)) return 1;

  state->Q = parameters->lambda0 * hubble_rate * state->thermo.activation * rho_flux;
  state->delta_Q_over_Q =
      delta_theta_over_theta +
      delta_rho_flux_over_rho_flux +
      state->thermo.dln_activation_dln_temperature * delta_temperature_over_temperature;
  return 0;
}
