# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
import scipy
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import NonlinearHeatElement1D
from pyfem.discretization import NonlinearFEModel
from pyfem.discretization import DirichletBoundary, NeumannBoundary
from pyfem.numeric import stationaryLoadControl
from pyfem.postprocessing import ConvergencePlot

# ---------------------------------------------------------
# Geometry and material
length              = 1.0
capacity            = lambda u: 1.0
dCapacity           = lambda u: 0.0
#conductivity        = lambda u: 1.0
#dConductivity       = lambda u: 0.0
conductivity        = lambda u: 1.0 + 1.0 * np.exp( -(u-0.5)**2 / 0.005 )
dConductivity       = lambda u: 1.0 * np.exp( -(u-0.5)**2 / 0.005 ) * (2.0/0.005)*(0.5-u)

# Load
load                = lambda x: 0.0
boundaryFlux        = 2.0

# Discretization
nElements           = 10
polynomialOrder     = 3

# Integration
integrationOrder    = 3 * (2*polynomialOrder + 1)

# Iteration
numberOfIncrements  = 5
numberOfCorrections = 30
tolerance           = 1e-5

# ---------------------------------------------------------
# General initialization
samples         = np.linspace( 0, length, num=100 )
fig             = plt.figure( )
axes            = ( fig.add_subplot( 2,1,1 ),
                    fig.add_subplot( 2,1,2 ))

# ---------------------------------------------------------
# Initialize FE model
model               = NonlinearFEModel( nElements*polynomialOrder + 1 )

# Create elements
basisFunctions      = IntegratedHierarchicBasisFunctions( polynomialOrder=polynomialOrder )
model.elements      = [ NonlinearHeatElement1D( capacity,
                                                dCapacity,
                                                conductivity,
                                                dConductivity,
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

rightBCID   = model.addBoundaryCondition(   NeumannBoundary(    nElements*polynomialOrder,
                                                                boundaryFlux) )

# Solve
u = stationaryLoadControl(  model,
                            np.zeros(model.size),
                            loadFactors=np.linspace(0.0, 1.0, num=numberOfIncrements+1),
                            maxCorrections=numberOfCorrections,
                            tolerance=tolerance,
                            verbose=True,
                            axes=axes[1],
                            convergencePlot=ConvergencePlot()   )

# Plot conductivity
samples = np.linspace( 0.0, np.max(model.sample( u, samples )), num=len(samples) )
axes[0].plot( samples, [conductivity(temp) for temp in samples] )
axes[0].set_xlabel( "T [C]" )
axes[0].set_ylabel( r'$\kappa$' + " [W/K]" )
axes[0].set_title( "Conductivity(temperature)" )

plt.tight_layout()
plt.show()