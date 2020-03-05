# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import LinearHeatElement1D
from pyfem.discretization import FEModel
from pyfem.numeric import separableFirstOrderThetaScheme

# ---------------------------------------------------------
# Geometry and material
length              = 1.0
capacity            = 1.0
conductivity        = 1.0

# Load
load                = lambda t, x: np.sin(x*np.pi)

# Discretization
time                = np.linspace(0.0, 10.0, 50)
nElements           = 10
polynomialOrder     = 3

# Integration
integrationOrder    = 2*polynomialOrder + 1

# ---------------------------------------------------------
# Initialize FE model
model               = FEModel( nElements*polynomialOrder + 1 )

# Create elements
basisFunctions      = IntegratedHierarchicBasisFunctions( polynomialOrder=polynomialOrder )
initialLoad         = lambda x: load( time[0], x )
model.elements      = [ LinearHeatElement1D(    capacity,
                                                conductivity,
                                                (i*length/nElements, (i+1)*length/nElements),
                                                np.asarray( range(i*polynomialOrder, (i+1)*polynomialOrder+1) ),
                                                initialLoad,
                                                basisFunctions=basisFunctions,
                                                integrationOrder=integrationOrder   ) 
                        for i in range(nElements) ]

# Integrate
model.allocateZeros( )
model.integrate( )

# Boundary conditions
penaltyValue    = 1e3
model.applyDirichletBoundary(   0, 
                                0.0, 
                                penaltyValue=penaltyValue )

#model.applyDirichletBoundary(   nElements*polynomialOrder, 
#                                0.0, 
#                                penaltyValue=penaltyValue )
model.applyNeumannBoundary(     nElements*polynomialOrder,
                                0.0)

# Solve
initialSolution     = np.zeros( model.size )
timeHistory = separableFirstOrderThetaScheme( time, initialSolution, model, load )

# Output
samples     = np.linspace( 0, length, num=100 )

for solution in timeHistory:
    values      = model.sample( solution, samples )
    plt.plot( samples, values, '.-' )
    
plt.show()