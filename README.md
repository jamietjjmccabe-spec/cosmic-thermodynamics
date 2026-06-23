# Cosmic thermodynamics — Registration freeze-in

Active branch: `stage4-class-registration`.

Implemented on that branch:

- a compiled C Standard-Model EOS/transfer kernel;
- the covariant source `Q=lambda0 H A(T) rho_flux`;
- the full local perturbation `delta Q/Q` including the temperature/trace term;
- regression tests for QCD-centred activation and the local time-shift identity;
- the exact insertion and regression contract for a writable CLASS fork.

Run:

```bash
make test
```

This repository does not yet claim a completed finite-k CLASS spectrum.
