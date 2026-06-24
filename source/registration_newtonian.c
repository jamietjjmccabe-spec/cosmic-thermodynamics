#include "registration_newtonian.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static void set_error(char *buffer, size_t size, const char *message) {
  if (buffer != NULL && size > 0) {
    snprintf(buffer, size, "%s", message);
  }
}

int registration_newtonian_derivs(
    const registration_eos_table *table,
    const registration_transfer_parameters *parameters,
    const registration_newtonian_input *input,
    registration_newtonian_output *output,
    char *error_message,
    size_t error_message_size) {
  if (table == NULL || parameters == NULL || input == NULL || output == NULL) {
    set_error(error_message, error_message_size, "null Newtonian derivative argument");
    return 1;
  }
  if (!(input->scale_factor > 0.0 &&
        input->conformal_hubble > 0.0 &&
        input->temperature_gev > 0.0 &&
        input->rho_flux > 0.0 &&
        input->rho_registration >= 0.0 &&
        input->k_squared >= 0.0)) {
    set_error(error_message, error_message_size,
              "invalid scale factor, Hubble rate, temperature, density, or k^2");
    return 1;
  }

  memset(output, 0, sizeof(*output));

  registration_thermo_state thermo;
  if (registration_eos_evaluate(
          table,
          input->temperature_gev,
          parameters->trace_power,
          parameters->trace_floor,
          &thermo,
          error_message,
          error_message_size)) {
    return 1;
  }

  const double thermal_response =
      4.0 + thermo.dln_g_rho_dln_temperature;
  if (fabs(thermal_response) < 1.0e-12) {
    set_error(error_message, error_message_size,
              "thermal density response is singular");
    return 1;
  }

  output->delta_temperature_over_temperature =
      input->delta_rho_thermal_over_rho_thermal / thermal_response;

  output->delta_theta_over_theta =
      (input->theta_transfer -
       3.0 * input->psi_prime -
       3.0 * input->conformal_hubble * input->phi) /
      (3.0 * input->conformal_hubble);

  const double physical_hubble =
      input->conformal_hubble / input->scale_factor;
  const double delta_flux_over_flux =
      input->delta_flux / input->rho_flux;

  if (registration_transfer_evaluate(
          table,
          parameters,
          input->temperature_gev,
          physical_hubble,
          input->rho_flux,
          output->delta_theta_over_theta,
          delta_flux_over_flux,
          output->delta_temperature_over_temperature,
          &output->transfer,
          error_message,
          error_message_size)) {
    return 1;
  }

  output->delta_Q =
      output->transfer.Q * output->transfer.delta_Q_over_Q;

  if (output->transfer.Q > 0.0) {
    output->birth_flow_ratio_defined = 1;
    output->birth_flow_ratio_estimate =
        sqrt(input->k_squared) * fabs(input->delta_flux) /
        (input->scale_factor * output->transfer.Q);
  } else {
    output->birth_flow_ratio_defined = 0;
    output->birth_flow_ratio_estimate = NAN;
  }

  output->delta_flux_prime =
      -input->scale_factor *
      (output->delta_Q + output->transfer.Q * input->phi);

  output->delta_rho_registration_prime =
      -3.0 * input->conformal_hubble * input->delta_rho_registration -
      input->q_registration +
      3.0 * input->rho_registration * input->psi_prime +
      input->scale_factor *
          (output->delta_Q + output->transfer.Q * input->phi);

  output->q_registration_prime =
      -4.0 * input->conformal_hubble * input->q_registration +
      input->rho_registration * input->k_squared * input->phi -
      input->k_squared * input->delta_flux;

  output->energy_bianchi_residual =
      output->delta_rho_registration_prime +
      3.0 * input->conformal_hubble * input->delta_rho_registration +
      input->q_registration -
      3.0 * input->rho_registration * input->psi_prime +
      output->delta_flux_prime;

  output->momentum_balance_residual =
      output->q_registration_prime +
      4.0 * input->conformal_hubble * input->q_registration -
      input->rho_registration * input->k_squared * input->phi +
      input->k_squared * input->delta_flux;

  return 0;
}
