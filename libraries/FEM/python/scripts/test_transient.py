# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import LinearHeatElement1D
from pyfem.discretization import TransientFEModel
from pyfem.discretization import DirichletBoundary, NeumannBoundary
from pyfem.numeric import solveLinearHeat1D
from pyfem.postprocessing.graphics import animateTimeSeries

# ---------------------------------------------------------
# Geometry and material
length                      = 1.0
capacity                    = 1.0
conductivity                = 1.0

# Load
load                        = lambda t, x: 0.0

# Discretization
time                        = np.linspace(0.0, 1.0, 200)
nElements                   = 10
polynomialOrder             = 2

# Integration
integrationOrder            = 2*polynomialOrder + 1
finiteDifferenceImplicity   = 0.5

# ---------------------------------------------------------
# Initialize FE model
model               = TransientFEModel( nElements*polynomialOrder + 1, loadFunction=load )

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
model.addBoundaryCondition( DirichletBoundary(  0, 
                                                lambda t: 0.0   ) )

model.addBoundaryCondition( NeumannBoundary(    nElements*polynomialOrder,
                                                lambda t: 1.0) )

# Solve
initialSolution     = np.zeros( model.size )
timeSeries          = solveLinearHeat1D(    time, 
                                            initialSolution, 
                                            model, 
                                            theta=finiteDifferenceImplicity )

# Output
samples             = np.linspace( 0, length, num=100 )
animateTimeSeries(  time, 
                    samples, 
                    timeSeries, 
                    model,
                    speed=0.05,
                    ylim=( -0.1, 1.0 ) )

plt.show()