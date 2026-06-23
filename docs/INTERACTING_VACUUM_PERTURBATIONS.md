# Interacting-vacuum finite-k perturbation contract

## Sign convention

Use

\[
T_V^{\mu\nu}=-Vg^{\mu\nu},\qquad \nabla_\mu T_{\rm reg}^{\mu\nu}=Q_{\rm reg}^\nu=\nabla^\nu V.
\]

Define the positive transfer rate measured in the plasma frame by

\[
\mathcal Q\equiv-u^{\rm tr}_\nu Q_{\rm reg}^\nu=-u_{\rm tr}^\mu\nabla_\mu V.
\]

For vacuum decay, `mathcal Q > 0`, so

\[
\bar V'=-a\bar{\mathcal Q},\qquad \bar\rho_{\rm reg}'+3\mathcal H\bar\rho_{\rm reg}=a\bar{\mathcal Q}.
\]

## Newtonian gauge

For

\[
ds^2=a^2[-(1+2\phi)d\tau^2+(1-2\psi)d\mathbf x^2]
\]

and

\[
u_{\rm tr}^\mu=a^{-1}(1-\phi,v_{\rm tr}^i),
\]

perturbing `u_tr^mu nabla_mu V = -mathcal Q` gives

\[
\delta V'=-a(\delta\mathcal Q+\bar{\mathcal Q}\phi).
\]

For the trace-triggered law

\[
\mathcal Q=\lambda_0\frac{\Theta_{\rm tr}}{3}A(T)V,
\]

\[
\frac{\delta\mathcal Q}{\bar{\mathcal Q}}=
\frac{\delta\Theta_{\rm tr}}{\bar\Theta_{\rm tr}}+
\frac{\delta V}{\bar V}+
\frac{d\ln A}{d\ln T}\frac{\delta T}{\bar T}.
\]

The exchange-vector components are

\[
Q_{\rm reg}^0=\frac{1}{a}[\bar{\mathcal Q}+\delta\mathcal Q-\bar{\mathcal Q}\phi],
\]

\[
Q_{\rm reg}^i=a^{-2}\partial^i\delta V.
\]

The spatial gradient is therefore fixed, but this alone is not yet the complete registration Euler equation.

## Pressureless registration equations

Define the registration density contrast `delta_reg=delta rho_reg/bar rho_reg` and velocity divergence `theta_reg=partial_i v_reg^i`, with Fourier convention `nabla^2 -> -k^2`.

Then

\[
\delta_{\rm reg}'=-\theta_{\rm reg}+3\psi'
+\frac{a\bar{\mathcal Q}}{\bar\rho_{\rm reg}}
\left(\frac{\delta\mathcal Q}{\bar{\mathcal Q}}+\phi-\delta_{\rm reg}\right).
\]

The Euler equation is

\[
\theta_{\rm reg}'=-\mathcal H\theta_{\rm reg}+k^2\phi
-\frac{a\bar{\mathcal Q}}{\bar\rho_{\rm reg}}\theta_{\rm reg}
-\frac{k^2}{\bar\rho_{\rm reg}}\delta V.
\]

These equations follow from projecting `nabla_mu T_reg^{mu nu}=nabla^nu V` parallel and orthogonal to the registration four-velocity. Signs must be translated to the exact CLASS Fourier and velocity-divergence conventions before coding.

An equivalent non-fractional continuity equation is

\[
\delta\rho_{\rm reg}'+3\mathcal H\delta\rho_{\rm reg}
+\bar\rho_{\rm reg}\theta_{\rm reg}-3\bar\rho_{\rm reg}\psi'=-\delta V'.
\]

This form is useful for numerical conservation checks.

## Momentum-transfer decomposition

Relative to the plasma frame,

\[
Q_{\rm reg}^\mu=\mathcal Q u_{\rm tr}^\mu+F_{\rm tr}^\mu,
\qquad u^{\rm tr}_\mu F_{\rm tr}^\mu=0,
\]

with

\[
F_{\rm tr}^\mu=h_{\rm tr}^{\mu\nu}\nabla_\nu V.
\]

Thus `Q_reg^mu = mathcal Q u_tr^mu` is a special restriction equivalent to vanishing spatial vacuum gradient in the plasma frame. It is not implied by the interacting-vacuum stress tensor.

## Closure status

The effective linear Newtonian-gauge system is closed only after supplying:

1. the plasma EOS and `delta T` mapping;
2. `delta Theta_tr` including metric and velocity terms;
3. adiabatic initial data for `delta V`;
4. Einstein equations and all standard species;
5. the synchronous-gauge form and a gauge-regression test.

The equations above provide an effective interacting-vacuum closure, not a microscopic action.

## Four-form caveat

A source-free four-form in four dimensions has no local propagating mode, but its ordinary field equation fixes its dual field strength to a spacetime constant. A genuinely local `V(x)` therefore requires additional structure: a coupling, membrane/source sector, constrained order parameter, derivative terms, or another explicit action-level mechanism. Coupling a QCD trace scalar to a four-form is a candidate construction, not yet a derived result.
