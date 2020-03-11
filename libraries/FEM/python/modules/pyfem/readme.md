# PyFEM

This module is meant to be a framework for supporting proofs-of-concept of the FEM-based optimal control of PDEs (especially the heat equation) using adjoint formulations. To put it mildly, performance is not a primary aspect of this framework.

## Class Structure

In most cases, I tried to create an object-oriented structure but you might find some exceptions. Anyway, here's brief overview of the structure:

* [*Integrator*](https://github.com/matekelemen/CiE/blob/optcontrol/libraries/FEM/python/modules/pyfem/numeric/integration.py) is responsible for integrating a scalar, single variate input function on a specified domain. The current implementation uses gaussian quadrature with user-selectable polynomial order.

* [*BasisFunctions*](https://github.com/matekelemen/CiE/blob/optcontrol/libraries/FEM/python/modules/pyfem/discretization/basisfunctions.py): 1D matching set of basis functions defined on the [-1,1] domain. The functions in this class form a basis for their respective function spaces. *BasisFunctions* is responsible for providing an interface for the evaluation of its functions at one or several points, and evaluating to 0 when sampled out of bounds. Currently, all scripts use integrated legendre basis functions (even though a polynomial order of 1 is used, meaning that these basis functions are identical to the classic linear ones).

* [*Element*](https://github.com/matekelemen/CiE/blob/optcontrol/libraries/FEM/python/modules/pyfem/discretization/element.py) has instances of *BasisFunctions* (1 in 1D) that it maps to its own domain, a list of degrees-of-freedom that maps their local DoFs to the global DoFs the *Element* belongs to, and an instance of *Integrator*. *Element* is responsible for computing its own stiffness matrix, mass matrix, and load vector, as well as updating the global ones. Also, it is responsible for computing the solution field (temperature field for example) in its own domain by evaluating its basis functions weighted with coefficients.

* [*BoundaryCondition*](https://github.com/matekelemen/CiE/tree/optcontrol/libraries/FEM/python/modules/pyfem/discretization/boundaryconditions.py) might not be the best abstraction, so it may be removed in the future. Currently its only function is to store the global DoF it acts on, and the value of the boundary condition (and also a penalty value depending on the type of the boundary condition). Since different physical models require different enforcement of boundary conditions, this task is delegated to *FEModel*.

* [*FEModel*](https://github.com/matekelemen/CiE/tree/optcontrol/libraries/FEM/python/modules/pyfem/discretization/femodel.py) has a list of *Element*s and *BoundaryCondition*s, and is responsible for computing the global structural matrices and the load vector, along with modifying them to enforce boundary conditions (*TransientFEModel*s have time-dependent boundaries and time-and-space-dependent loads). *FEModel* is also responsible for evaluating the solution field (heat field for example), given a set of coefficients for the global DoFs, by evaluating the individual contributions of all elements on their respective domains. Note that *FEModel* **is not responsible for the assembly** process, meaning that you have to come up with the distribution of global DoFs yourself and pass subsets of it to *Element*s such that they are connected the way you want them to be.


This concludes the basic structure of classes for the FE framework, which might be expanded in the future.


## Time Integration

For now, time integration is performed specifically for the linear poisson equation, using a linear combination of explicit and implicit stepping. 
```math
 \frac{a}{b} 
 ```