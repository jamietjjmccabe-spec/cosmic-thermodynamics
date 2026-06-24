# Cosmic thermodynamics — Registration freeze-in

Active branch: `stage4-class-registration`.

Implemented on that branch:

- a compiled C Standard-Model EOS/transfer kernel;
- the covariant source `Q=lambda0 H A(T) rho_flux`;
- the full local perturbation `delta Q/Q` including expansion, vacuum-density, and temperature/trace terms;
- EOS logarithmic derivatives needed for non-conformal temperature perturbations;
- a regular Newtonian-gauge finite-k derivative kernel evolving `delta V`, absolute `delta rho_reg`, and momentum density `q_reg`;
- exact energy-Bianchi and momentum-balance residuals;
- regressions for the QCD-centred development activation, local time-shift identity, `lambda0=0`, and `rho_reg=0` regularity;
- Newtonian and synchronous analytical contracts plus adiabatic initial data;
- a source-specific map for later insertion into CLASS.

Run:

```bash
make test
```

The regular Newtonian kernel is compiled and unit-tested. This repository does not yet claim a completed CLASS build or finite-k CMB spectrum because the connected runtime has no writable/full CLASS source tree.
