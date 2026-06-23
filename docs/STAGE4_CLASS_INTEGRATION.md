# Stage 4 CLASS integration contract

Implemented here is an independently compiled C kernel for

\[
Q=\lambda_0 H A(T)\rho_\Phi,\qquad A(T)=|1-g_p/g_\rho|^n,
\]

and

\[
\delta Q/Q=\delta\Theta/\Theta+\delta\rho_\Phi/\rho_\Phi+(d\ln A/d\ln T)\,\delta T/T.
\]

## Upstream insertion points

- `include/background.h`: add `rho_reg`, `rho_flux`, transfer parameters, EOS storage.
- `source/input.c`: parse `Omega0_reg`, `Omega0_flux`, `registration_lambda0`, `registration_trace_power`, EOS path, and an external-clock test switch.
- `source/background.c`: evolve `rho_reg'=-3 Hc rho_reg+aQ` and `rho_flux'=-3 Hc(1+w_flux)rho_flux-aQ` with ordinary CDM set to zero.
- `include/perturbations.h`, `source/perturbations.c`: add `delta_reg`, `theta_reg`, `delta_flux`, `theta_flux` and derive energy/momentum transfer from `Q_reg^mu=Q u_rad^mu`.

The first restrictive flux closure is `w_flux=-1`, `cs2_flux=1`, `pi_flux=0`.

## Mandatory gates

1. `lambda0=0` reproduces the uncoupled reference.
2. `Omega0_cdm=0` in replacement runs.
3. Background source and sink close exactly.
4. Perturbed energy and momentum transfer close.
5. `k -> 0` agrees with the standalone separate-universe audit.
6. The external-clock control reproduces the known entropy failure.
7. `rho_reg a^3` is constant after freeze-in.
8. Physical spectra agree between synchronous and Newtonian gauge.
9. Results converge under time-step, interpolation, and tolerance scans.

A writable CLASS fork was not connected, so this branch is a validated integration workspace, not a claim that finite-k CLASS spectra have already been produced.
