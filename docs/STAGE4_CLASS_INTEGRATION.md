# Stage 4 CLASS integration contract

Implemented here is an independently compiled C kernel for

\[
Q=\lambda_0 H A(T)\rho_\Phi,\qquad A(T)=|1-g_p/g_\rho|^n,
\]

and

\[
\delta Q/Q=\delta\Theta/\Theta+\delta\rho_\Phi/\rho_\Phi+(d\ln A/d\ln T)\,\delta T/T.
\]

## Important interpretation limits

The measured activation peak near `0.16992 GeV` is inherited from the chosen development EOS table, which uses a sharp 170 MeV QCD-transition branch. It is not yet an independent prediction of the model and must be re-tested with a modern smooth lattice-QCD equation of state.

The CLASS mapping must not use either of the shortcuts `deltaTheta/Theta = theta_rad/theta_tot` or `deltaT/T = delta_gamma/4` through the QCD crossover.

For an equilibrium plasma

\[
\rho_{\rm th}(T)=\frac{\pi^2}{30}g_\rho(T)T^4,
\]

so the local temperature perturbation is

\[
\frac{\delta T}{T}=\frac{\delta\rho_{\rm th}/\rho_{\rm th}}{4+d\ln g_\rho/d\ln T}.
\]

The familiar `delta_gamma/4` limit is recovered only when the relevant thermal bath follows the photon temperature and `g_rho` is locally constant.

The transfer expansion scalar is the scalar

\[
\Theta_{\rm tr}=\nabla_\mu u_{\rm tr}^\mu,
\]

with `u_tr^mu` fixed to the radiation/plasma energy frame. In conformal Newtonian gauge, using the usual scalar-velocity convention,

\[
\frac{\delta\Theta_{\rm tr}}{\bar\Theta}
=\frac{\theta_{\rm tr}-3\phi'-3\mathcal H\psi}{3\mathcal H}.
\]

In synchronous gauge,

\[
\frac{\delta\Theta_{\rm tr}}{\bar\Theta}
=\frac{\theta_{\rm tr}+h'/2}{3\mathcal H}.
\]

Signs must be checked against the exact CLASS velocity-divergence convention. The radiation-frame velocity itself is the enthalpy-weighted plasma velocity, not a ratio of velocity divergences.

## Upstream insertion points

- `include/background.h`: add `rho_reg`, `rho_flux`, transfer parameters, EOS storage.
- `source/input.c`: parse `Omega0_reg`, `Omega0_flux`, `registration_lambda0`, `registration_trace_power`, EOS path, and an external-clock test switch.
- `source/background.c`: evolve `rho_reg'=-3 Hc rho_reg+aQ` and `rho_flux'=-3 Hc(1+w_flux)rho_flux-aQ` with ordinary CDM set to zero.
- `include/perturbations.h`, `source/perturbations.c`: add `delta_reg`, `theta_reg`, `delta_flux`, `theta_flux`; reconstruct the local plasma temperature perturbation from the thermal EOS; construct `deltaTheta_tr` including metric and velocity terms; and derive energy/momentum transfer from `Q_reg^mu=Q u_rad^mu`.

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
10. Replacing the development EOS table with a smooth lattice-QCD EOS does not qualitatively alter the production history.

A writable CLASS fork was not connected, so this branch is a validated integration workspace, not a claim that finite-k CLASS spectra have already been produced.
