#ifndef REGISTRATION_NEWTONIAN_H
#define REGISTRATION_NEWTONIAN_H

#include "registration_transfer.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  double scale_factor;
  double conformal_hubble;
  double k_squared;
  double temperature_gev;

  double rho_flux;
  double rho_registration;

  double delta_flux;
  double delta_rho_registration;
  double q_registration;

  double phi;
  double psi_prime;
  double theta_transfer;
  double delta_rho_thermal_over_rho_thermal;
} registration_newtonian_input;

typedef struct {
  registration_transfer_state transfer;

  double delta_theta_over_theta;
  double delta_temperature_over_temperature;
  double delta_Q;

  double delta_flux_prime;
  double delta_rho_registration_prime;
  double q_registration_prime;

  double energy_bianchi_residual;
  double momentum_balance_residual;

  /* Linear Fourier-mode estimate of |Q_spatial|/Q_energy during active
     creation. This is a perturbative linearity diagnostic, not a
     pointwise nonlinear proof that Q_mu Q^mu < 0. */
  int birth_flow_ratio_defined;
  double birth_flow_ratio_estimate;
} registration_newtonian_output;

int registration_newtonian_derivs(
    const registration_eos_table *table,
    const registration_transfer_parameters *parameters,
    const registration_newtonian_input *input,
    registration_newtonian_output *output,
    char *error_message,
    size_t error_message_size);

#ifdef __cplusplus
}
#endif

#endif
