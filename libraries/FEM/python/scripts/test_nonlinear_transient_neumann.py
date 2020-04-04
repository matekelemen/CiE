# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import NonlinearHeatElement1D
from pyfem.discretization import NonlinearTransientFEModel
from pyfem.discretization import DirichletBoundary, NeumannBoundary
from pyfem.numeric import solveNonlinearHeat1D
from pyfem.postprocessing import ConvergencePlot
from pyfem.postprocessing.graphics import animateTimeSeries

# ---------------------------------------------------------
# Geometry and material
length                      = 1.0
#capacity                    = lambda u: 1.0
#dCapacity                   = lambda u: 0.0
capacity                    = lambda u: 1.0 + 9.0 * np.exp( -(u-0.5)**2 / 0.005 )
dCapacity                   = lambda u: 9.0 * np.exp( -(u-0.5)**2 / 0.005 ) * (2.0/0.005)*(0.5-u)
#conductivity                = lambda u: 1.0
#dConductivity               = lambda u: 0.0
conductivity                = lambda u: 1.0 + 9.0 * np.exp( -(u-0.5)**2 / 0.005 )
dConductivity               = lambda u: 9.0 * np.exp( -(u-0.5)**2 / 0.005 ) * (2.0/0.005)*(0.5-u)

# Load
load                        = lambda t, x: 0.0
boundaryFlux                = lambda t: 1.0
penaltyValue                = 1e10

# Discretization
time                        = np.linspace(0.0, 1.0, 60)
nElements                   = 10
polynomialOrder             = 3

# Integration
integrationOrder            = 1 * (2*polynomialOrder + 1)
finiteDifferenceImplicity   = 0.5

# Iteration
numberOfIncrements          = 1
numberOfCorrections         = 50
tolerance                   = 1e-5

# ---------------------------------------------------------
# General initialization
samples         = np.linspace( 0, length, num=100 )
convergencePlot = ConvergencePlot()

loadFactors     = None
if numberOfIncrements > 1:
    loadFactors = np.linspace( 0.0, 1.0, num=numberOfIncrements )
else:
    loadFactors = [1.0]

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
                                                lambda t: 0.0,
                                                penaltyValue=penaltyValue   ) )

model.addBoundaryCondition( NeumannBoundary(    nElements*polynomialOrder,
                                                boundaryFlux) )

# Initial values
initialSolution     = np.zeros(model.size)
#model.updateTime(   time[0], 
#                    u,
#                    geometricStiffness=False )
#
#initialStiffness    = model.stiffness
#initialMass         = model.mass
#initialLoad         = model.load
#
#timeSeries          = np.zeros( (len(time), *u.shape) )
#timeSeries[0]       = u.copy()
#
## ---------------------------------------------------------
## Iteration
#for k in range(1, len(time)):
#
#    # Update time
#    initialStiffness    = model.stiffness
#    initialMass         = model.mass
#    initialLoad         = model.load
#    model.updateTime(   time[k], 
#                        u,
#                        geometricStiffness=False )
#
#    # Solve
#    u = nonlinearSolver(    model,
#                            u,
#                            initialStiffness,
#                            initialMass,
#                            initialLoad,
#                            time[k] - time[k-1],
#                            loadFactors=np.linspace(0.0, 1.0, num=numberOfIncrements+1),
#                            maxCorrections=numberOfCorrections,
#                            tolerance=tolerance,
#                            verbose=True,
#                            axes=None,
#                            convergencePlot=convergencePlot   )
#
#    timeSeries[k] = u.copy()

timeSeries          = solveNonlinearHeat1D( time,
                                            initialSolution,
                                            model,
                                            theta=finiteDifferenceImplicity,
                                            loadFactors=loadFactors,
                                            maxCorrections=numberOfCorrections,
                                            tolerance=tolerance,
                                            verbose=True,
                                            convergencePlot=convergencePlot )

# ---------------------------------------------------------
# Plot
animateTimeSeries(  time, 
                    samples, 
                    timeSeries, 
                    model,
                    speed=0.02,
                    ylim=( -0.1, 1.0 ) )
plt.tight_layout()
plt.show( block=True )