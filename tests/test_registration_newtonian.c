#include "registration_newtonian.h"

#include <math.h>
#include <stdio.h>

#define CHECK(condition, message) \
  do { \
    if (!(condition)) { \
      fprintf(stderr, "FAIL: %s\n", message); \
      return 1; \
    } \
  } while (0)

static int almost_zero(double x, double scale) {
  return fabs(x) <= 2.0e-13 * fmax(1.0, scale);
}

int main(int argc, char **argv) {
  const char *path =
      (argc > 1) ? argv[1] : "data/sm_thermodynamics_husdal2016.csv";
  char error[256] = {0};

  registration_eos_table table;
  CHECK(registration_eos_load_csv(path, &table, error, sizeof(error)) == 0,
        error);

  registration_transfer_parameters parameters = {
      .lambda0 = 820.6438184870336,
      .trace_power = 2.0,
      .trace_floor = 1.0e-14,
  };

  registration_newtonian_input input = {
      .scale_factor = 2.0e-12,
      .conformal_hubble = 4.0e-3,
      .k_squared = 2.5e-5,
      .temperature_gev = 0.171,
      .rho_flux = 8.0e34,
      .rho_registration = 3.0e29,
      .delta_flux = -2.0e25,
      .delta_rho_registration = 4.0e20,
      .q_registration = -7.0e16,
      .phi = 1.0e-5,
      .psi_prime = -2.0e-9,
      .theta_transfer = 9.0e-9,
      .delta_rho_thermal_over_rho_thermal = -4.0e-5,
  };

  registration_newtonian_output output;
  CHECK(registration_newtonian_derivs(
            &table, &parameters, &input, &output, error, sizeof(error)) == 0,
        error);

  const double expected_delta_theta =
      (input.theta_transfer - 3.0 * input.psi_prime -
       3.0 * input.conformal_hubble * input.phi) /
      (3.0 * input.conformal_hubble);
  CHECK(fabs(output.delta_theta_over_theta - expected_delta_theta) < 1.0e-16,
        "Newtonian expansion-scalar mapping failed");

  const double expected_delta_temperature =
      input.delta_rho_thermal_over_rho_thermal /
      (4.0 + output.transfer.thermo.dln_g_rho_dln_temperature);
  CHECK(fabs(output.delta_temperature_over_temperature -
             expected_delta_temperature) < 1.0e-16,
        "non-conformal temperature mapping failed");

  const double energy_scale =
      fabs(output.delta_rho_registration_prime) +
      fabs(output.delta_flux_prime) + 1.0;
  const double momentum_scale =
      fabs(output.q_registration_prime) +
      fabs(input.k_squared * input.delta_flux) + 1.0;
  CHECK(almost_zero(output.energy_bianchi_residual, energy_scale),
        "linear energy Bianchi residual failed");
  CHECK(almost_zero(output.momentum_balance_residual, momentum_scale),
        "linear momentum balance residual failed");

  const double expected_birth_ratio =
      sqrt(input.k_squared) * fabs(input.delta_flux) /
      (input.scale_factor * output.transfer.Q);
  CHECK(output.birth_flow_ratio_defined == 1,
        "active transfer did not define birth-flow diagnostic");
  CHECK(fabs(output.birth_flow_ratio_estimate - expected_birth_ratio) <
            1.0e-14 * fmax(1.0, fabs(expected_birth_ratio)),
        "birth-flow diagnostic mapping failed");

  registration_newtonian_input zero_density = input;
  zero_density.rho_registration = 0.0;
  zero_density.delta_rho_registration = 0.0;
  zero_density.q_registration = 0.0;
  registration_newtonian_output zero_output;
  CHECK(registration_newtonian_derivs(
            &table, &parameters, &zero_density, &zero_output,
            error, sizeof(error)) == 0,
        "regular variables failed at zero registration density");
  CHECK(isfinite(zero_output.delta_rho_registration_prime) &&
        isfinite(zero_output.q_registration_prime),
        "zero-density derivatives are not finite");

  registration_transfer_parameters disabled = parameters;
  disabled.lambda0 = 0.0;
  registration_newtonian_input uncoupled = input;
  uncoupled.delta_flux = 0.0;
  registration_newtonian_output uncoupled_output;
  CHECK(registration_newtonian_derivs(
            &table, &disabled, &uncoupled, &uncoupled_output,
            error, sizeof(error)) == 0,
        error);
  CHECK(uncoupled_output.transfer.Q == 0.0 &&
        uncoupled_output.delta_Q == 0.0 &&
        uncoupled_output.delta_flux_prime == 0.0,
        "lambda0=0 did not disable vacuum transfer");
  CHECK(uncoupled_output.birth_flow_ratio_defined == 0 &&
        isnan(uncoupled_output.birth_flow_ratio_estimate),
        "birth-flow diagnostic should be undefined when Q=0");

  const double expected_uncoupled_density_prime =
      -3.0 * uncoupled.conformal_hubble *
          uncoupled.delta_rho_registration -
      uncoupled.q_registration +
      3.0 * uncoupled.rho_registration * uncoupled.psi_prime;
  CHECK(fabs(uncoupled_output.delta_rho_registration_prime -
             expected_uncoupled_density_prime) <
            1.0e-12 * fmax(1.0, fabs(expected_uncoupled_density_prime)),
        "lambda0=0 density equation did not reduce correctly");

  printf("PASS\n");
  printf("deltaTheta_over_Theta=%.12g\n",
         output.delta_theta_over_theta);
  printf("deltaT_over_T=%.12g\n",
         output.delta_temperature_over_temperature);
  printf("deltaQ_over_Q=%.12g\n",
         output.transfer.delta_Q_over_Q);
  printf("energy_bianchi_residual=%.12g\n",
         output.energy_bianchi_residual);
  printf("momentum_balance_residual=%.12g\n",
         output.momentum_balance_residual);
  printf("birth_flow_ratio_estimate=%.12g\n",
         output.birth_flow_ratio_estimate);
  printf("zero_density_delta_rho_prime=%.12g\n",
         zero_output.delta_rho_registration_prime);
  printf("zero_density_q_prime=%.12g\n",
         zero_output.q_registration_prime);

  registration_eos_free(&table);
  return 0;
}
