#ifndef REGISTRATION_TRANSFER_H
#define REGISTRATION_TRANSFER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  size_t size;
  double *log_temperature;
  double *g_rho;
  double *g_p;
  double *g_s;
} registration_eos_table;

typedef struct {
  double lambda0;
  double trace_power;
  double trace_floor;
} registration_transfer_parameters;

typedef struct {
  double g_rho;
  double g_p;
  double g_s;
  double trace_fraction;
  double activation;
  double dln_g_rho_dln_temperature;
  double dln_g_p_dln_temperature;
  double dln_g_s_dln_temperature;
  double dln_activation_dln_temperature;
} registration_thermo_state;

typedef struct {
  double Q;
  double delta_Q_over_Q;
  registration_thermo_state thermo;
} registration_transfer_state;

int registration_eos_load_csv(
    const char *path,
    registration_eos_table *table,
    char *error_message,
    size_t error_message_size);

void registration_eos_free(registration_eos_table *table);

int registration_eos_evaluate(
    const registration_eos_table *table,
    double temperature_gev,
    double trace_power,
    double trace_floor,
    registration_thermo_state *state,
    char *error_message,
    size_t error_message_size);

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
    size_t error_message_size);

#ifdef __cplusplus
}
#endif

#endif
