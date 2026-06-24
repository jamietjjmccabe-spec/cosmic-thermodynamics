# Cold mono-kinetic registration closure

## Status

The first finite-k CMB implementation adopts a deliberately restrictive effective hypothesis:

\[
\mathcal C_{\rm create}(x,p)
=\mathcal S_N(x)\,\delta_{\rm shell}
\!\left(p^\mu-m_{\rm reg}u_{\rm birth}^\mu\right).
\]

This is an ansatz, not a result derived from the first moment of the source.

The exchange vector fixes only

\[
Q_{\rm reg}^\mu
=\int p^\mu\mathcal C_{\rm create}\,dP
=\nabla^\mu V.
\]

Under the cold mono-kinetic ansatz, and only when the exchange vector is future timelike,

\[
u_{\rm birth}^\mu
=\frac{Q_{\rm reg}^\mu}{\sqrt{-Q_{{\rm reg}\,\alpha}Q_{\rm reg}^\alpha}},
\qquad
m_{\rm reg}\mathcal S_N=\sqrt{-Q_{{\rm reg}\,\alpha}Q_{\rm reg}^\alpha}.
\]

The resulting linear single-stream stress tensor is pressureless. This justifies the existing regular variables

\[
\delta V,\qquad \delta\rho_{\rm reg},\qquad q_{\rm reg}.
\]

It does not justify a warm or multi-stream hierarchy.

## Causal-source condition

For a non-negative local source of future-directed causal particles, the first source moment must itself be future causal:

\[
Q_{{\rm reg}\,\mu}Q_{\rm reg}^\mu\le0,
\qquad Q_{\rm reg}^{\hat0}>0.
\]

A spacelike first moment cannot be repaired merely by widening a positive particle momentum distribution.

However, in linear perturbation theory the spatial-gradient contribution to `Q_mu Q^mu` is quadratic in perturbation amplitude. Therefore a first-order Fourier code cannot establish the exact pointwise nonlinear condition by evaluating the invariant norm mode by mode.

During active production, the Newtonian kernel reports the diagnostic

\[
\epsilon_{\rm birth}(k,\tau)
=\frac{k\,|\delta V|}{a\,\bar{\mathcal Q}}.
\]

This estimates the spatial-to-temporal transfer ratio for a Fourier amplitude. The interpretation is:

- `epsilon_birth << 1`: compatible with a small timelike birth-velocity perturbation;
- `epsilon_birth ~ 1`: cold linearization is becoming marginal;
- `epsilon_birth > 1`: warning that the mode cannot be trusted as a small perturbation of the homogeneous timelike transfer;
- `Qbar = 0`: the ratio is undefined because no particles are being created and no birth velocity is physically required.

This is a linearity and consistency diagnostic, not an exact real-space causal proof. The exact test must be applied to reconstructed nonlinear fields or a real-space Vlasov/N-body realization.

## Zero-transfer limit

The normalized expression for `u_birth` is undefined when `Q_reg^mu=0`. This is not a physical singularity: when the creation kernel vanishes, no birth velocity needs to be assigned.

Code must therefore evolve the moment variables directly and must never divide by the transfer norm outside an active-production window.

The adiabatic initial condition

\[
\delta V=\bar V'\Delta\tau_{\rm ad}
\]

ensures that `delta V` vanishes with the background source in the deep early-time limit. Numerical runs must still verify that `epsilon_birth` remains controlled across the tails of the production episode.

## Validity range

The cold closure is valid for linear cosmology and single-stream evolution. It ceases to be a complete description after shell crossing, where coarse-grained velocity dispersion and anisotropic stress are generated. Halo calculations must then use Vlasov or particle phase-space evolution.

## Mandatory tests before interpreting a CMB spectrum

1. `epsilon_birth` remains much smaller than unity over the active production window for all linear modes used in the spectra.
2. Results converge when the active-window threshold and starting time are varied.
3. The cold source remains non-negative in the chosen local production model.
4. A narrow finite-width comparison source is eventually used to bound sensitivity to warm injection.
5. No claim about nonlinear timelikeness is made from first-order Fourier data alone.
