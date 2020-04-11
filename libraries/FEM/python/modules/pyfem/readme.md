# PyFEM

This module is meant to be a framework for supporting proofs-of-concept of the FEM-based optimal control of PDEs (especially the heat equation) using adjoint formulations. To put it mildly, performance is not a primary aspect of this framework.


# FE Formulation

## Stationary Heat Conduction

Let's start with a 1D stationary nonlinear heat conduction with Neumann boundaries:
```math
\frac{d}{dx}( \kappa \frac{du}{dx} ) = \lambda f                \\
\frac{du}{dx}\bigg\rvert_{x_0} = \lambda \Phi_0                 \\
\frac{du}{dx}\bigg\rvert_{x_1} = \lambda \Phi_1                 
\tag{1}
```
* $`u = u(x)`$ temperature field
* $`\kappa = \kappa (u(x))`$ conductivity
* $`f=f(u(x))`$ load
* $`\lambda`$ load factor

Unpacking the derivative using the chain rule yields:
```math
\frac{d\kappa}{dx}\frac{du}{dx} + \kappa \frac{d^2u}{dx^2}
- \lambda f
= 0         \tag{2}
```


```math
\frac{d\kappa}{du} (\frac{du}{dx})^2 + \kappa \frac{d^2u}{dx^2}
- \lambda f
= 0         \tag{3}
```

Choose an appropriate function space $`V`$ and assume that the solution $`u = \sum\limits_i N_i \hat{u}_i \in V`$ and the test function $`v = \sum\limits_i N_i \in V`$, then multiply by the test function $`v`$ and integrate over the domain $`\Omega=[x_0, x_1]`$:

```math
\int_\Omega v \frac{d\kappa}{du} (\frac{du}{dx})^2 d\Omega + \int_\Omega \kappa v \frac{d^2u}{dx^2} d\Omega 
- \int_\Omega \lambda v f d\Omega
= 0         \tag{4}
```

To sneak in the boundaries and get rid of the second derivatives, the second term on the LHS of $`(5)`$ has to be partially integrated (a threeway partial integration):
```math
\int_\Omega v \frac{d\kappa}{du} (\frac{du}{dx})^2 d\Omega
+ \bigg[\kappa v \frac{du}{dx} \bigg]_{x_0}^{x_1}
- \int_\Omega v \frac{d\kappa}{du} (\frac{du}{dx})^2 d\Omega
- \int_\Omega \kappa \frac{dv}{dx} \frac{du}{dx}d\Omega
- \lambda \int_\Omega v f d\Omega
= 0         \tag{5}
```

```math
\int_\Omega \kappa \frac{dv}{dx} \frac{du}{dx}d\Omega
- \lambda (\kappa_1 v_1 \Phi_1 - \kappa_0 v_0 \Phi_0)
+ \lambda \int_\Omega v f d\Omega
= 0         \tag{6}
```

* $`\kappa_0=\kappa(u(x_0))`$
* $`\kappa_1=\kappa(u(x_1))`$

The solutions at the boundaries $`u_0, u_1`$ are of course still unknown. Substituting the discrete counterparts of $`u`$ and $`v`$  in $`(6)`$ yields:

```math
\int_\Omega \kappa \frac{dN_i}{dx} \frac{dN_j}{dx} d\Omega \hat{u}_i
- \lambda (\kappa_1 N_i(x_1) \Phi_1 - \kappa_0 N_i(x_0) \Phi_0)
+ \lambda \int_\Omega f N_i
= 0         \tag{7}
```

At this point, the stiffness matrix and the load vector could be computed if $`\kappa`$ was constant, and the system could be solved. However, we're focusing on the nonlinear system and have to proceed with some nonlinear theory.

From now on, the LHS in $`(7)`$ will be referred to as the residual $`r=r(u,\lambda)`$, and will be the basis of the analysis. The problem boils down to finding the roots of this residual for specified load factors, which is done by a combination of explicit ODE integration (predictor) and some variation of the Newton method (corrector). Unfortunately, we'll need the derivatives for the Newton method.

The partial derivatives of the residual form the tangent stiffness matrix $`\bar K`$ and load vector $`q`$:

```math
\bar{K}_{ij} = \frac{\partial r_i}{\partial \hat{u}_j}
= 
\int_\Omega \kappa \frac{dN_i}{dx}\frac{dN_j}{dx}d\Omega
+ \int_\Omega \frac{d\kappa}{du}N_j\frac{dN_i}{dx}\frac{dN_k}{dx}d\Omega \hat{u}_k 
+ \lambda \int_\Omega \frac{\partial f}{\partial \hat u_j}N_i N_j d\Omega
\tag{8}
```

```math
q_i = -\frac{\partial r_i}{\partial \lambda}
=
- \int_\Omega f N_i d\Omega + \bigg( \kappa_1 N_i(x_1) \Phi_1 - \kappa_0 N_i(x_0) \Phi_0 \bigg)
\tag{9}
```

For the sake if simplicity, the load $`f`$ will be assumed to be independent of $`u`$. Then, the **tangent stiffness matrix $`\bar{K}`$** can be broken down into two components: the **stiffness matrix $`K`$** and the **geometric stiffness matrix $`K_g`$**.

```math
\bar{K} = K + K_g
\tag{10}
```

* $`K       = \int_\Omega \kappa \frac{dN_i}{dx}\frac{dN_j}{dx}d\Omega`$
* $`K_g     = \int_\Omega \frac{d\kappa}{du}N_j\frac{dN_i}{dx}\frac{dN_k}{dx} d\Omega \hat{u}_k  `$

*Notes: $`K_g`$ is pretty expensive to compute at each iteration while not contributing all that much to the convergence of the Newton iteration. In some cases (with nonzero source loads for example) it even prevents the residual from converging. Maybe it's just a bug in the code but right now, neglecting $`K_g`$ and using only $`K`$ is much faster and more reliable.*



## Alternative to the Newton method

Since the Newton method comes with a lot of extra computational burdens while not ensuring convergence, it's a good idea to try cheaper alternatives. One possible method is converting the problem into a fixed point iteration:

```math
\Phi(\hat u)
=
K(\hat u)\hat u - f(\hat u) 
= 0     \tag{11}
```

Of course it might not converge either, but at least we don't have to compute the geometric stiffness $`K_g`$ for example. Also, the iteration might converge to a different solution, but we can detect most of those cases by keeping an eye on the residual, which is supposed to approach $`0`$.

The most scary situation would be converging to a solution that **is** in equilibrium ($`r=0`$), but on a different equilibrium path, which would indicate bifurcations or very tricky nonlinearities. Thankfully, we don't have to worry about bifurcations for heat problems (to the extent of my knowledge).


## Transient Heat Conduction

The formulation of the transient 1D nonlinear heat conduction will be mostly identical to the stationary case, but with the added time derivative term:

```math
-c \frac{\partial u}{\partial t} + \frac{\partial }{\partial x}(\kappa \frac{\partial u}{\partial x}) - \lambda f 
= 0     
\\
\frac{\partial u}{\partial x} \bigg \rvert_{x_0} = \lambda \Phi_0(t)
\\
\frac{\partial u}{\partial x} \bigg \rvert_{x_1} = \lambda \Phi_1(t)
\\
u(x,0) = u_0(x)
\tag{12}
```

* $`u = u(x, t)`$
* $`c = c(u)`$
* $`\kappa = \kappa(u)`$

To reduce clutter, the following notation is intruduced:
* $`\dot \square    := \frac{\partial \square}{\partial t}`$
* $`\square'        := \frac{\partial \square}{\partial x}`$

Expand the derivatives:
```math
-c \dot u + \frac{d\kappa}{du} (u')^2 + \kappa u'' - \lambda f
= 0
\tag{13}
```

Multiply with the test function $`v`$ and integrate over the space domain:
```math
-\int_\Omega c v \dot u d\Omega
+ \int_\Omega \frac{d\kappa}{du} v (u')^2 d\Omega
+ \int_\Omega \kappa v u'' d\Omega
- \lambda \int_\Omega f v d\Omega
= 0
\tag{14}
```

Integrate by parts:
```math
-\int_\Omega c v \dot u d\Omega
+ \bigg[ \kappa v u' \bigg]_{x_0}^{x_1}
- \int_\Omega \kappa v' u' d\Omega
- \lambda \int_\Omega f v d\Omega
= 0
\tag{15}
```

Substituting the discrete counterparts of $`u`$ and $`v`$ works the same way as in the stationary case, except the coefficients are now time-dependent $`\hat u_i = \hat u_i (t)`$.
```math
\int_\Omega c N_i N_j d\Omega \dot{\hat u}_i
+ \int_\Omega \kappa N_i' N_j' d\Omega \hat u_i
- \lambda \bigg( \kappa_1N_i(x_1)\Phi_1 - \kappa_0 N_i(x_0) \Phi_0 \bigg)
+ \lambda \int_\Omega f N_i d\Omega
= 0
\tag{16}
```

Before formulating the residual $`r(u,\lambda)`$, we have to discretize in time as well. However, the current notation is a bit hard to work with so from this point, I'm just going to use the universal notation for "structural" matrices and the load vector:
* $`M   := \int_\Omega c N_i N_j d\Omega`$
* $`K   := \int_\Omega \kappa N_i' N_j' d\Omega`$
* $`q   := - \bigg( \kappa_1N_i(x_1)\Phi_1 - \kappa_0 N_i(x_0) \Phi_0 \bigg) + \int_\Omega f N_i d\Omega`$

Keep in mind that these entities are not truly derived from the residuals yet. For example, the "stiffness matrix" $`K_{ij}`$ is obviously not equal to $`\frac{\partial r_i}{\partial \hat u_j}`$ due to the non-constant material properties $`c(u)`$ and $`\kappa(u)`$, but we'll use them for the time being to keep things readable.

Now we can discretize in time as well, which we'll do using a mixed explicit-implicit finite difference scheme. The basic idea is as follows:
```math
\frac{dF(t)}{dt} = G(t,F(t)) 
\rightarrow
\frac{F^{k+1} - F^{k} }{ \Delta t } 
\approx
\theta G(t^{k+1}, F^{k+1}) + (1-\theta) G(t^k, F^k)
\tag{17}
```

* $`\Delta t := t^{k+1} - t^k`$
* $`\square^k := \square(t^k)`$
* $`\theta \in [0,1]`$


Apply $`(17)`$ to $`(16)`$:
```math
\frac{\hat u^{k+1} - \hat u^k}{\Delta t}
+ \theta (M^{k+1})^{^{-1}} \bigg( K^{k+1} \hat u^{k+1} - \lambda^{k+1} q^{k+1} \bigg)
+ (1-\theta)M^{k^{-1}} \bigg( K^k \hat u^k - \lambda^k q^k  \bigg)
= 0
\tag{18}
```

To sort out this mess, we need to separate the unknown terms ($`\square^{k+1}`$) from the terms that we already computed ($`\square^k`$):
```math
\bigg[
    \bigg( \theta (M^{k+1})^{^{-1}} K^{k+1} + \frac{1}{\Delta t} I \bigg) \hat u^{k+1} - \theta (M^{k+1})^{^{-1}} \bigg( \lambda^{k+1} q^{k+1} \bigg)
\bigg]
+
\bigg[
    (1-\theta) M^{k^{-1}} \bigg( K^k \hat u^k - \lambda^k q^k \bigg)
    - \frac{1}{\Delta t} \hat u^k
\bigg]
= 0
\tag{19}
```

*Several things to note here:*
*First of all, it might seem weird to carry the load factors ($`\lambda`$) through the time steps but if we don't want to impose extra constraints on the nonlinear iteration later, we have to keep using them. The other painful thing is that the mass matrix ($`M`$) has to be inverted at every time step and every nonlinear iteration step. This is a huge difference compared to the solution of the linear system, where the mass matrix is constant, and can just premultiply the affected terms without ever having to invert it. As reference, here's how $`(19)`$ looks like in the linear case (premultiplied by $`M`$):*
```math
\bigg[
    \bigg( \theta K + \frac{1}{\Delta t}M \bigg) \hat u^{k+1}
\bigg]
+
\bigg[
    \bigg( (1-\theta) K - \frac{1}{\Delta t} M \bigg)
    \hat u^k
    - (1-\theta) q^k
    - \theta q^{k+1}
\bigg]
= 0
\tag{20}
```

This is where my knowledge currently ends. I'm working on implementing a fixed point iteration to solve the nonlinear transient model, but that's still in progress. I'll update this section once I have useful results.

# Module

## Class Structure

In most cases, I tried to create an object-oriented structure but you might find some exceptions. Anyway, here's brief overview:

* [*Integrator*](https://gitlab.com/matekelemen/CiE/-/tree/optcontrol/libraries/FEM/python/modules/pyfem/numeric/integration.py) is responsible for integrating a scalar, single variate input function on a specified domain. The current implementation uses gaussian quadrature with user-selectable polynomial order.

* [*BasisFunctions*](https://gitlab.com/matekelemen/CiE/-/tree/optcontrol/libraries/FEM/python/modules/pyfem/discretization/basisfunctions.py): 1D matching set of basis functions defined on the [-1,1] domain. The functions in this class form a basis for their respective function spaces. *BasisFunctions* is responsible for providing an interface for the evaluation of its functions at one or several points, and evaluating to 0 when sampled out of bounds. Currently, all scripts use integrated legendre basis functions (even though a polynomial order of 1 is used, meaning that these basis functions are identical to the classic linear ones).

* [*Element*](https://gitlab.com/matekelemen/CiE/-/tree/optcontrol/libraries/FEM/python/modules/pyfem/discretization/element.py) has instances of *BasisFunctions* (1 in 1D) that it maps to its own domain, a list of degrees-of-freedom that maps their local DoFs to the global DoFs the *Element* belongs to, and an instance of *Integrator*. *Element* is responsible for computing its own stiffness matrix, mass matrix, and load vector, as well as updating the global ones. Also, it is responsible for computing the solution field (temperature field for example) in its own domain by evaluating its basis functions weighted with coefficients.

* [*BoundaryCondition*](https://gitlab.com/matekelemen/CiE/-/tree/optcontrol/libraries/FEM/python/modules/pyfem/discretization/boundaryconditions.py) might not be the best abstraction, so it may be removed in the future. Currently its only function is to store the global DoF it acts on, and the value of the boundary condition (and also a penalty value depending on the type of the boundary condition). Since different physical models require different enforcement of boundary conditions, this task is delegated to *FEModel*.

* [*FEModel*](https://gitlab.com/matekelemen/CiE/-/tree/optcontrol/libraries/FEM/python/modules/pyfem/discretization/femodel.py) has a list of *Element*s and *BoundaryCondition*s, and is responsible for computing the global structural matrices and the load vector, along with modifying them to enforce boundary conditions (*TransientFEModel*s have time-dependent boundaries and time-and-space-dependent loads). *FEModel* is also responsible for evaluating the solution field (heat field for example), given a set of coefficients for the global DoFs, by evaluating the individual contributions of all elements on their respective domains. Note that *FEModel* **is not responsible for the assembly** process, meaning that you have to come up with the distribution of global DoFs yourself and pass subsets of it to *Element*s such that they are connected the way you want them to be.


This concludes the basic structure of classes for the FE framework, which might be expanded in the future.