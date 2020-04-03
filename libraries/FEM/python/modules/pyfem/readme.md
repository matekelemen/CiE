# PyFEM

This module is meant to be a framework for supporting proofs-of-concept of the FEM-based optimal control of PDEs (especially the heat equation) using adjoint formulations. To put it mildly, performance is not a primary aspect of this framework.


## FE Discretization

Let's start with a 1D stationary nonlinear heat equation with Neumann boundaries:
```math
\frac{d}{dx}( \kappa \frac{du}{dx} ) = \lambda f                \\
\frac{du}{dx}\bigg\rvert_{x_0} = \lambda \Phi_0                 \\
\frac{du}{dx}\bigg\rvert_{x_1} = \lambda \Phi_1                 \tag{1}
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
\frac{d\kappa}{du}\frac{1}{\frac{du}{dx}}\frac{du}{dx} + \kappa \frac{d^2u}{dx^2}
- \lambda f
= 0         \tag{3}
```


```math
\frac{d\kappa}{du} + \kappa \frac{d^2u}{dx^2} 
- \lambda f
= 0         \tag{4}
```

Choose an appropriate function space $`V`$ and assume that the solution $`u = \sum\limits_i N_i \hat{u}_i \in V`$ and the test function $`v = \sum\limits_i N_i \in V`$, then multiply by the test function $`v`$ and integrate over the domain $`\Omega=[x_0, x_1]`$:

```math
\int_\Omega v \frac{d\kappa}{du} d\Omega + \int_\Omega \kappa v \frac{d^2u}{dx^2} d\Omega 
- \int_\Omega \lambda v f d\Omega
= 0         \tag{5}
```

To sneak in the boundaries and get rid of the second derivatives, the second term on the LHS of $`(5)`$ has to be partially integrated (a threeway partial integration):
```math
\int_\Omega v \frac{d\kappa}{du} d\Omega
+ [\kappa v \frac{du}{dx}]\bigg\rvert_{x_0}^{x_1}
- \int_\Omega v \frac{d\kappa}{du}d\Omega
- \int_\Omega \kappa \frac{dv}{dx} \frac{du}{dx}d\Omega
- \lambda \int_\Omega v f d\Omega
= 0         \tag{6}
```

```math
\int_\Omega \kappa \frac{dv}{dx} \frac{du}{dx}d\Omega
- \lambda (\kappa_1 v_1 \Phi_1 - \kappa_0 v_0 \Phi_0)
+ \lambda \int_\Omega v f d\Omega
= 0         \tag{7}
```

* $`\kappa_0=\kappa(u(x_0))`$
* $`\kappa_1=\kappa(u(x_1))`$

The solutions at the boundaries $`u_0, u_1`$ are of course still unknown. Substituting the discrete counterparts of $`u`$ and $`v`$  in $`(7)`$ yields:

```math
\int_\Omega \kappa \frac{dN_i}{dx} \frac{dN_j}{dx} d\Omega \hat{u}_i
- \lambda (\kappa_1 N_i(x_1) \Phi_1 - \kappa_0 N_i(x_0) \Phi_0)
+ \lambda \int_\Omega f N_i
= 0         \tag{8}
```

At this point, the stiffness matrix and the load vector could be computed if $`\kappa`$ was constant, and the system could be solved. However, we're focusing on the nonlinear system and have to proceed with some nonlinear theory.

From now on, the LHS in $`(8)`$ will be referred to as the residual $`r=r(u,\lambda)`$, and will be the basis of the analysis. The problem boils down to finding the roots of this residual for specified load factors, which is done by a combination of explicit ODE integration (predictor) and some variation of the Newton method (corrector). Unfortunately, we'll need the derivatives for the Newton method.

The partial derivatives of the residual form the tangent stiffness matrix $`K`$ and load vector $`q`$:

```math
\bar{K}_{ij} = \frac{\partial r_i}{\partial \hat{u}_j}
= 
\int_\Omega \kappa \frac{dN_i}{dx}\frac{dN_j}{dx}d\Omega
+ \int_\Omega \frac{d\kappa}{du}N_j\frac{dN_i}{dx}\frac{dN_k}{dx}d\Omega \hat{u}_k 
+ \lambda \int_\Omega \frac{\partial f}{\partial u}N_i N_j d\Omega
\tag{9}
```

```math
q_i = -\frac{\partial r_i}{\partial \lambda}
=
\int_\Omega f N_i d\Omega - (\kappa_1 N_i(x_1) \Phi_1 - \kappa_0 N_i(x_0) \Phi_0)
\tag{10}
```

For the sake if simplicity, the load $`f`$ will be assumed to be independent of $`u`$. Then, the **tangent stiffness matrix $`\bar{K}`$** can be broken down into two components: the **stiffness matrix $`K`$** and the **geometric stiffness matrix $`K_g`$**.

```math
\bar{K} = K + K_g
\tag{11}
```

* $`K       = \int_\Omega \kappa \frac{dN_i}{dx}\frac{dN_j}{dx}d\Omega`$
* $`K_g     = \int_\Omega \frac{d\kappa}{du}N_j\frac{dN_i}{dx}\frac{dN_k}{dx} d\Omega \hat{u}_k  `$

*Notes: $`K_g`$ is pretty expensive to compute at each iteration while not contributing all that much to the convergence of the Newton iteration. In some cases (with nonzero source loads for example) it even prevents the residual from converging. Maybe it's just a bug in the code but right now, neglecting $`K_g`$ and using only $`K`$ is much faster and more reliable.*

## Class Structure

In most cases, I tried to create an object-oriented structure but you might find some exceptions. Anyway, here's brief overview of the structure:

* [*Integrator*](https://github.com/matekelemen/CiE/blob/optcontrol/libraries/FEM/python/modules/pyfem/numeric/integration.py) is responsible for integrating a scalar, single variate input function on a specified domain. The current implementation uses gaussian quadrature with user-selectable polynomial order.

* [*BasisFunctions*](https://github.com/matekelemen/CiE/blob/optcontrol/libraries/FEM/python/modules/pyfem/discretization/basisfunctions.py): 1D matching set of basis functions defined on the [-1,1] domain. The functions in this class form a basis for their respective function spaces. *BasisFunctions* is responsible for providing an interface for the evaluation of its functions at one or several points, and evaluating to 0 when sampled out of bounds. Currently, all scripts use integrated legendre basis functions (even though a polynomial order of 1 is used, meaning that these basis functions are identical to the classic linear ones).

* [*Element*](https://github.com/matekelemen/CiE/blob/optcontrol/libraries/FEM/python/modules/pyfem/discretization/element.py) has instances of *BasisFunctions* (1 in 1D) that it maps to its own domain, a list of degrees-of-freedom that maps their local DoFs to the global DoFs the *Element* belongs to, and an instance of *Integrator*. *Element* is responsible for computing its own stiffness matrix, mass matrix, and load vector, as well as updating the global ones. Also, it is responsible for computing the solution field (temperature field for example) in its own domain by evaluating its basis functions weighted with coefficients.

* [*BoundaryCondition*](https://github.com/matekelemen/CiE/tree/optcontrol/libraries/FEM/python/modules/pyfem/discretization/boundaryconditions.py) might not be the best abstraction, so it may be removed in the future. Currently its only function is to store the global DoF it acts on, and the value of the boundary condition (and also a penalty value depending on the type of the boundary condition). Since different physical models require different enforcement of boundary conditions, this task is delegated to *FEModel*.

* [*FEModel*](https://github.com/matekelemen/CiE/tree/optcontrol/libraries/FEM/python/modules/pyfem/discretization/femodel.py) has a list of *Element*s and *BoundaryCondition*s, and is responsible for computing the global structural matrices and the load vector, along with modifying them to enforce boundary conditions (*TransientFEModel*s have time-dependent boundaries and time-and-space-dependent loads). *FEModel* is also responsible for evaluating the solution field (heat field for example), given a set of coefficients for the global DoFs, by evaluating the individual contributions of all elements on their respective domains. Note that *FEModel* **is not responsible for the assembly** process, meaning that you have to come up with the distribution of global DoFs yourself and pass subsets of it to *Element*s such that they are connected the way you want them to be.


This concludes the basic structure of classes for the FE framework, which might be expanded in the future.