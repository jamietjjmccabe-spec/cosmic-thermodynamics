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

## Independent linear failure diagnostics

The Newtonian kernel reports two independent active-production diagnostics.

The scalar production-linearity ratio is

\[
\epsilon_Q(k,\tau)
=\left|\frac{\delta\mathcal Q}{\bar{\mathcal Q}}\right|.
\]

This tests whether the local creation-rate perturbation remains small relative to the FLRW production rate.

The birth-flow ratio is

\[
\epsilon_{\rm birth}(k,\tau)
=\frac{k\,|\delta V|}{a\,\bar{\mathcal Q}}.
\]

This estimates the spatial-to-temporal transfer ratio for a Fourier amplitude.

The interpretation is:

- `epsilon_Q << 1`: the scalar production law remains perturbative;
- `epsilon_birth << 1`: compatible with a small timelike birth-flow perturbation;
- either ratio approaching unity: the corresponding linear closure is becoming marginal;
- `Qbar = 0`: both ratios are undefined because no particles are being created.

These diagnostics test different failure modes and must both be monitored.

## Causal-source condition

For a non-negative local source of future-directed causal particles, the first source moment must itself be future causal:

\[
Q_{{\rm reg}\,\mu}Q_{\rm reg}^\mu\le0,
\qquad Q_{\rm reg}^{\hat0}>0.
\]

A spacelike first moment cannot be repaired merely by widening a positive particle momentum distribution.

However, in linear perturbation theory the spatial-gradient contribution to `Q_mu Q^mu` is quadratic in perturbation amplitude. Therefore a first-order Fourier code cannot establish the exact pointwise nonlinear condition by evaluating the invariant norm mode by mode. `epsilon_birth` is a linearity and consistency diagnostic, not an exact real-space causal proof.

## Post-production decoupling diagnostic

The end of background production does not by itself eliminate the spatial vacuum-gradient force. The kernel therefore reports

\[
R_{\rm force}(k,\tau)
=
\frac{k^2|\delta V|}
{\mathcal H|q_{\rm reg}|+\bar\rho_{\rm reg}k^2|\phi|}.
\]

This compares the residual vacuum-gradient force with Hubble drag plus standard gravitational infall.

A CDM-like post-production limit requires

\[
R_{\rm force}\longrightarrow0
\]

for all relevant modes after freeze-in. The ratio is undefined when the comparison force in the denominator vanishes; code must report this explicitly rather than insert an arbitrary floor.

## Zero-transfer limit

The normalized expression for `u_birth` is undefined when `Q_reg^mu=0`. This is not a physical singularity: when the creation kernel vanishes, no birth velocity needs to be assigned.

Code must therefore evolve the moment variables directly and must never divide by the transfer norm outside an active-production window.

The adiabatic initial condition

\[
\delta V=\bar V'\Delta\tau_{\rm ad}
\]

ensures that `delta V` vanishes with the background source in the deep early-time limit. Numerical runs must still verify that the active-production diagnostics remain controlled across the production tails and that `R_force` decays afterward.

## Validity range

The cold closure is valid for linear cosmology and single-stream evolution. It ceases to be a complete description after shell crossing, where coarse-grained velocity dispersion and anisotropic stress are generated. Halo calculations must then use Vlasov or particle phase-space evolution.

## Mandatory tests before interpreting a CMB spectrum

1. `epsilon_Q` remains much smaller than unity over the active production window.
2. `epsilon_birth` remains much smaller than unity over the active production window.
3. `R_force` tends to zero after freeze-in for all relevant modes.
4. Results converge when the active-window threshold and starting time are varied.
5. The cold source remains non-negative in the chosen local production model.
6. A narrow finite-width comparison source is eventually used to bound sensitivity to warm injection.
7. No claim about nonlinear timelikeness is made from first-order Fourier data alone.
