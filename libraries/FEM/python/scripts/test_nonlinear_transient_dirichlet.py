# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import NonlinearHeatElement1D
from pyfem.discretization import NonlinearTransientFEModel
from pyfem.discretization import DirichletBoundary
from pyfem.numeric import transientFixedPointIteration as nonlinearSolver
from pyfem.postprocessing import ConvergencePlot

# ---------------------------------------------------------
# Geometry and material
length                      = 1.0
capacity                    = lambda u: 1.0
dCapacity                   = lambda u: 0.0
#conductivity                = lambda u: 1.0
#dConductivity               = lambda u: 0.0
conductivity                = lambda u: 1.0 + 1.0 * np.exp( -(u-0.5)**2 / 0.005 )
dConductivity               = lambda u: 1.0 * np.exp( -(u-0.5)**2 / 0.005 ) * (2.0/0.005)*(0.5-u)

# Load
load                        = lambda t, x: 0.0
boundaryTemperature         = lambda t: 1.0

# Discretization
time                        = np.linspace(0.0, 1.0, 10)
nElements                   = 10
polynomialOrder             = 3

# Integration
integrationOrder            = 2 * (2*polynomialOrder + 1)
finiteDifferenceImplicity   = 0.5

# Iteration
numberOfIncrements  = 3
numberOfCorrections = 30
tolerance           = 1e-5

# ---------------------------------------------------------
# General initialization
samples         = np.linspace( 0, length, num=100 )

convergencePlot = ConvergencePlot()

# ---------------------------------------------------------
# Initialize FE model
model               = NonlinearTransientFEModel( nElements*polynomialOrder + 1, loadFunction=load )

# Create elements
basisFunctions      = IntegratedHierarchicBasisFunctions( polynomialOrder=polynomialOrder )
initialLoad         = lambda x: load( time[0], x )
model.elements      = [ NonlinearHeatElement1D( capacity, dCapacity,
                                                conductivity, dConductivity,
                                                (i*length/nElements, (i+1)*length/nElements),
                                                np.asarray( range(i*polynomialOrder, (i+1)*polynomialOrder+1) ),
                                                initialLoad,
                                                basisFunctions=basisFunctions,
                                                integrationOrder=integrationOrder   ) 
                        for i in range(nElements) ]

# Integrate
model.allocateZeros( )

# Boundary conditions
model.addBoundaryCondition( DirichletBoundary(  0, 
                                                lambda t: 0.0   ) )

model.addBoundaryCondition( DirichletBoundary(  nElements*polynomialOrder,
                                                boundaryTemperature) )

# Solve
u = nonlinearSolver(    model,
                        np.zeros(model.size),
                        0.1,
                        loadFactors=np.linspace(0.0, 1.0, num=numberOfIncrements+1),
                        maxCorrections=numberOfCorrections,
                        tolerance=tolerance,
                        verbose=True,
                        axes=None,
                        convergencePlot=convergencePlot   )

plt.tight_layout()
plt.show()