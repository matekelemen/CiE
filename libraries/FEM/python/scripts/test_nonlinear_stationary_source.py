# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import NonlinearHeatElement1D
from pyfem.discretization import FEModel
from pyfem.discretization import DirichletBoundary
from pyfem.numeric import stationaryLoadControl

# ---------------------------------------------------------
# Geometry and material
length              = 1.0
capacity            = lambda u: 1.0
conductivity        = lambda u: 1.0 + 9.0 * np.exp( -(u-0.5)**2 / 0.005 )

# Load
load                = lambda x: 15.0 * np.sin(np.pi/length*x)
boundaryTemperature = 2.0

# Discretization
nElements           = 15
polynomialOrder     = 2

# Integration
integrationOrder    = 2 * (2*polynomialOrder + 1)

# Iteration
numberOfIncrements  = 6
numberOfCorrections = 5
tolerance           = 1e-12

# ---------------------------------------------------------
# General initialization
samples         = np.linspace( 0, length, num=100 )
fig             = plt.figure( )
axes            = ( fig.add_subplot( 2,1,1 ),
                    fig.add_subplot( 2,1,2 ))

# ---------------------------------------------------------
# Initialize FE model
model               = FEModel( nElements*polynomialOrder + 1 )

# Create elements
basisFunctions      = IntegratedHierarchicBasisFunctions( polynomialOrder=polynomialOrder )
model.elements      = [ NonlinearHeatElement1D( capacity,
                                                conductivity,
                                                (i*length/nElements, (i+1)*length/nElements),
                                                np.asarray( range(i*polynomialOrder, (i+1)*polynomialOrder+1) ),
                                                load,
                                                basisFunctions=basisFunctions,
                                                integrationOrder=integrationOrder   ) 
                        for i in range(nElements) ]

model.allocateZeros( )

# Boundary conditions (right BC is the load)
leftBCID    = model.addBoundaryCondition(   DirichletBoundary(  0, 
                                                                0.0 ))

rightBCID   = model.addBoundaryCondition(   DirichletBoundary(  nElements*polynomialOrder,
                                                                0.0) )

# Set functionals
loadFunctional      = lambda controlParameter: lambda x: controlParameter*load(x)
boundaryManipulator = lambda controlParameter: None

# Solve
u = stationaryLoadControl(  model,
                            np.zeros(model.size),
                            loadFunctional=loadFunctional,
                            boundaryFunctional=boundaryManipulator,
                            maxIncrements=numberOfIncrements,
                            maxCorrections=numberOfCorrections,
                            tolerance=1e-8,
                            verbose=True,
                            axes=axes[1]   )


# Plot conductivity
samples = np.linspace( 0.0, np.max(model.sample( u, samples )), num=len(samples) )
axes[0].plot( samples, [conductivity(temp) for temp in samples] )
axes[0].set_xlabel( "T [C]" )
axes[0].set_ylabel( r'$\kappa$' + " [W/K]" )

plt.show()