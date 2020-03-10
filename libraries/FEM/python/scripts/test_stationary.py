# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import LinearHeatElement1D
from pyfem.discretization import FEModel
from pyfem.discretization import DirichletBoundary, NeumannBoundary

# ---------------------------------------------------------
# Geometry and material
length              = 1.0
capacity            = 1.0
conductivity        = 1.0

# Load
load                = lambda x: np.sin(x*np.pi)

# Boundaries
penaltyValue    = 1e3

# Discretization
nElements           = 10
polynomialOrder     = 2

# Integration
integrationOrder    = 2*polynomialOrder + 1

# ---------------------------------------------------------
# Initialize FE model
model               = FEModel( nElements*polynomialOrder + 1 )

# Create elements
basisFunctions      = IntegratedHierarchicBasisFunctions( polynomialOrder=polynomialOrder )
model.elements      = [ LinearHeatElement1D(    capacity,
                                                conductivity,
                                                (i*length/nElements, (i+1)*length/nElements),
                                                np.asarray( range(i*polynomialOrder, (i+1)*polynomialOrder+1) ),
                                                load,
                                                basisFunctions=basisFunctions,
                                                integrationOrder=integrationOrder   ) 
                        for i in range(nElements) ]

# Integrate
model.allocateZeros( )
model.integrate( )

# Boundary conditions
leftBCID    = model.addBoundaryCondition(   DirichletBoundary(  0, 
                                                                0.0, 
                                                                penaltyValue=penaltyValue   ) )

rightBCID   = model.addBoundaryCondition(   NeumannBoundary(    nElements*polynomialOrder,
                                                                0.0) )

# Solve
u       = model.solveStationary()

# Output
samples = np.linspace( 0, length, num=100 )
values  = model.sample( u, samples )
plt.plot( samples, values, '.-' )
plt.show()