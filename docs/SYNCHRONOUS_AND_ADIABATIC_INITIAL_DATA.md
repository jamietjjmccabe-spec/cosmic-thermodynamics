# Synchronous-gauge equations and adiabatic initial data

## Synchronous metric and transfer source

Use

\[
ds^2=a^2[-d\tau^2+(\delta_{ij}+h_{ij})dx^idx^j]
\]

with scalar trace perturbation `h` and

\[
u_{\rm tr}^\mu=a^{-1}(1,v_{\rm tr}^i).
\]

For

\[
\mathcal Q=\lambda_0\frac{\Theta_{\rm tr}}{3}A(T)V,
\]

\[
\frac{\delta\Theta_{\rm tr}}{\bar\Theta_{\rm tr}}=
\frac{\theta_{\rm tr}+h'/2}{3\mathcal H},
\]

and

\[
\frac{\delta\mathcal Q}{\bar{\mathcal Q}}=
\frac{\theta_{\rm tr}+h'/2}{3\mathcal H}+
\frac{\delta V}{\bar V}+
\frac{d\ln A}{d\ln T}\frac{\delta T}{\bar T}.
\]

The interacting-vacuum equation is

\[
\delta V'=-a\,\delta\mathcal Q.
\]

## Registration equations

For pressureless registration matter,

\[
\delta_{\rm reg}'=-\theta_{\rm reg}-\frac{h'}{2}
+\frac{a\bar{\mathcal Q}}{\bar\rho_{\rm reg}}
\left(\frac{\delta\mathcal Q}{\bar{\mathcal Q}}-\delta_{\rm reg}\right),
\]

\[
\theta_{\rm reg}'=-\mathcal H\theta_{\rm reg}
-\frac{a\bar{\mathcal Q}}{\bar\rho_{\rm reg}}\theta_{\rm reg}
-\frac{k^2}{\bar\rho_{\rm reg}}\delta V.
\]

These fractional equations are singular when `rho_reg -> 0`. The implementation should therefore evolve absolute perturbations during production:

\[
\delta\rho_{\rm reg}'+3\mathcal H\delta\rho_{\rm reg}
+q_{\rm reg}+\frac{1}{2}\bar\rho_{\rm reg}h'=a\delta\mathcal Q=-\delta V',
\]

where

\[
q_{\rm reg}\equiv\bar\rho_{\rm reg}\theta_{\rm reg},
\]

and

\[
q_{\rm reg}'+4\mathcal H q_{\rm reg}=-k^2\delta V.
\]

The absolute system remains regular for vanishing initial registration abundance and provides a direct perturbative Bianchi check.

## Adiabatic initial condition

The physical adiabatic mode is a common local displacement along the background trajectory. For any local background scalars `X` and `Y`,

\[
\frac{\delta X}{X'}=\frac{\delta Y}{Y'}\equiv\Delta\tau_{\rm ad},
\]

up to the sign convention chosen for the coordinate shift.

Thus the vacuum initial condition is

\[
\boxed{\delta V_{\rm ini}=\bar V'_{\rm ini}\Delta\tau_{\rm ad}}
\]

or equivalently

\[
\boxed{\delta V_{\rm ini}=\frac{\bar V'_{\rm ini}}{\bar\rho_{\rm th,ini}'}\,\delta\rho_{\rm th,ini}}
\]

and

\[
\delta V_{\rm ini}=\frac{\bar V'_{\rm ini}}{\bar T'_{\rm ini}}\,\delta T_{\rm ini}.
\]

Since `V'=-a Q`, this condition drives `delta V -> 0` when the conversion rate vanishes. An arbitrary finite `delta V` in that limit is an independent vacuum-isocurvature mode and must not be included in the minimal adiabatic run.

For registration,

\[
\delta\rho_{\rm reg,ini}=\bar\rho_{\rm reg,ini}'\Delta\tau_{\rm ad}
=(a\bar{\mathcal Q}-3\mathcal H\bar\rho_{\rm reg})_{\rm ini}\Delta\tau_{\rm ad}.
\]

When `rho_reg` is negligible this reduces to

\[
\delta\rho_{\rm reg,ini}\simeq a\bar{\mathcal Q}\Delta\tau_{\rm ad}.
\]

The regular momentum mode is selected by removing the homogeneous decaying solution:

\[
q_{\rm reg}(\tau)=-a^{-4}(\tau)\int^{\tau}d\tilde\tau\,a^4(\tilde\tau)k^2\delta V(\tilde\tau).
\]

At a sufficiently early starting time this implies `q_reg,ini` is negligible, but convergence under earlier starts must be checked.

## Temperature evolution

For entropy conservation,

\[
a^3s(T)=\mathrm{const},\qquad s(T)\propto g_s(T)T^3,
\]

so

\[
\frac{T'}{T}=-\frac{\mathcal H}{1+\frac13 d\ln g_s/d\ln T}.
\]

This relation should be used when converting the adiabatic time displacement into `delta T` through a changing Standard-Model EOS.

## CLASS gauge-fixing constraint

Current CLASS explicitly rejects synchronous-gauge runs without CDM because CDM is used to define the initial timelike hypersurface and remove the residual synchronous gauge freedom.

Therefore an exact `Omega_cdm=0` replacement test cannot use unmodified CLASS synchronous gauge. The primary implementation should use Newtonian gauge. A later synchronous regression requires one of:

1. a negligible spectator geodesic dust component followed by an `Omega_spectator -> 0` convergence test;
2. an auxiliary geodesic clock with negligible stress energy;
3. a modification of CLASS gauge fixing that does not rely on physical CDM.

The plasma-comoving condition is not a drop-in replacement because a pressure-supported plasma is not geodesic.
