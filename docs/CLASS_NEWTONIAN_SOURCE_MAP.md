# CLASS Newtonian source map

Target inspected: `lesgourg/class_public`, `master`, `source/perturbations.c` blob `c78fe4376d2eeebfbf4ff0cfca13b048d4513fa0` and `include/perturbations.h` blob `74b3a2aad1263c6c516491fcaf768351ad55cd88`.

This document maps the already compiled `registration_newtonian_derivs()` kernel onto current CLASS internals. It is an insertion contract, not a claim that upstream CLASS has been built in this repository.

## Required background additions

`background.h` and `background.c` must first expose at least:

- `has_registration`;
- `index_bg_rho_registration`;
- `index_bg_rho_flux`;
- `index_bg_temperature_sm` or an equivalent entropy-conserving temperature map;
- transfer parameters and EOS table storage;
- background equations

\[
V'=-a\mathcal Q,
\qquad
\rho_{\rm reg}'=-3\mathcal H\rho_{\rm reg}+a\mathcal Q.
\]

Without these entries, perturbations cannot read a self-consistent `V`, `rho_reg`, or `T` from `pvecback`.

## `include/perturbations.h`

Inside `struct perturbations_vector`, add indices for the regular integrated variables:

```c
int index_pt_delta_V;
int index_pt_delta_rho_registration;
int index_pt_q_registration;
```

Do not use `delta_registration` and `theta_registration` during freeze-in because they divide by a background density that can vanish.

Inside `struct perturbations_workspace`, add diagnostics if runtime monitoring is desired:

```c
double registration_energy_bianchi_residual;
double registration_momentum_balance_residual;
```

## `perturbations_vector_init()`

When `pba->has_registration == _TRUE_`, allocate the three new scalar variables in this order:

```c
class_define_index(pv->index_pt_delta_V, index_pt, 1);
class_define_index(pv->index_pt_delta_rho_registration, index_pt, 1);
class_define_index(pv->index_pt_q_registration, index_pt, 1);
```

Copy them when approximation intervals change. The component should not switch off after freeze-in because its perturbations continue to gravitate.

## `perturbations_initial_conditions()`

For the minimal adiabatic mode initialize through a common time displacement:

\[
\delta V=V'\Delta\tau_{\rm ad},
\qquad
\delta\rho_{\rm reg}=\rho_{\rm reg}'\Delta\tau_{\rm ad}.
\]

Remove the homogeneous decaying momentum solution and initialize `q_registration` from the regular early-time series. In the limit `Q -> 0`, `delta V` must vanish. An arbitrary finite `delta V` is a vacuum-isocurvature mode and must not appear in the minimal run.

## `perturbations_total_stress_energy()`

Add registration after ordinary pressureless species and before the Einstein equations are solved:

```c
ppw->delta_rho += y[pv->index_pt_delta_rho_registration];
ppw->rho_plus_p_theta += y[pv->index_pt_q_registration];
ppw->rho_plus_p_tot += pvecback[pba->index_bg_rho_registration];
```

The vacuum contribution is

```c
ppw->delta_rho += y[pv->index_pt_delta_V];
ppw->delta_p   -= y[pv->index_pt_delta_V];
```

and contributes nothing to `rho_plus_p_theta` because `rho_V+p_V=0`.

## `perturbations_derivs()` in Newtonian gauge

CLASS already constructs:

- `metric_continuity = -3 phi_prime`;
- `metric_euler = k2 psi`;
- `pvecmetric[index_mt_psi]`;
- `pvecmetric[index_mt_phi_prime]`.

The standalone kernel uses the notation

- `phi` = Newtonian lapse potential = CLASS `psi`;
- `psi_prime` = derivative of spatial-curvature potential = CLASS `phi_prime`.

This notation translation must be explicit when filling `registration_newtonian_input`.

The plasma expansion input is

\[
\frac{\delta\Theta_{\rm tr}}{\bar\Theta}
=
\frac{\theta_{\rm tr}-3\phi'_{\rm CLASS}-3\mathcal H\psi_{\rm CLASS}}
{3\mathcal H}.
\]

The host code must construct `theta_transfer` as the enthalpy-weighted velocity divergence of the complete thermally coupled Standard-Model plasma represented by the EOS, not merely a late-time photon variable.

The thermal density perturbation must satisfy

\[
\frac{\delta T}{T}
=
\frac{\delta\rho_{\rm th}/\rho_{\rm th}}
{4+d\ln g_\rho/d\ln T}.
\]

After filling the input structure, call:

```c
registration_newtonian_derivs(
    &pba->registration_eos,
    &pba->registration_parameters,
    &reg_input,
    &reg_output,
    error_message,
    _ERRORMSGSIZE_);
```

Then assign:

```c
dy[pv->index_pt_delta_V] = reg_output.delta_flux_prime;
dy[pv->index_pt_delta_rho_registration] =
    reg_output.delta_rho_registration_prime;
dy[pv->index_pt_q_registration] = reg_output.q_registration_prime;
```

Monitor the returned residuals during development. They should remain near the ODE integration tolerance.

## Source functions and matter power

If registration is to replace CDM observationally, add its density and momentum to:

- total non-relativistic matter density;
- total matter velocity;
- `delta_m` and `theta_m` source construction;
- transfer-function output titles if individual registration outputs are requested;
- lensing and nonlinear-module matter definitions.

Do not add vacuum `V` to clustering matter.

## Regression order

1. Compile the modified CLASS tree.
2. Run with `registration_lambda0=0` and verify exact recovery of the reference noninteracting equations.
3. Run the interacting background with all perturbation sources disabled and reproduce Stage 1.
4. Evolve a very small `k/(aH)` mode and recover the separate-universe Stage 3 result.
5. Verify energy and momentum residuals at every accepted ODE step.
6. Produce Newtonian-gauge CMB spectra with `Omega_cdm=0`.
7. Add a controlled synchronous gauge anchor only for the later gauge-regression limit.

## Current execution boundary

The transfer, thermodynamic, and regular Newtonian derivative kernels compile and pass standalone tests. A complete CLASS build has not been executed because the connected runtime cannot clone GitHub and has no local CLASS source tree or installed CLASS binary.
