# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import LinearHeatElement1D
from pyfem.discretization import TransientFEModel
from pyfem.discretization import DirichletBoundary, NeumannBoundary
from pyfem.numeric import solveLinearHeat1D, solveAdjointLinearHeat1D
from pyfem.optcontrol import squaredSolutionErrorFunctional
from pyfem.postprocessing.graphics import animateTimeSeries

# ---------------------------------------------------------
# SETTINGS
# ---------------------------------------------------------
# Reference
referenceControl            = lambda t: 1.0

# Geometry and material
length                      = 1.0
capacity                    = 1.0
conductivity                = 1.0

# Load
load                        = lambda t, x: 0.0

# Discretization
time                        = np.linspace(0.0, 1.0, 50)
nElements                   = 50
polynomialOrder             = 2

# Integration
integrationOrder            = polynomialOrder + 5
finiteDifferenceImplicity   = 0.75

# Adjoint
numberOfAdjointIterations   = 50
regularization              = 100.0

# Postprocessing
numberOfSamples             = 100

# ---------------------------------------------------------
# REFERENCE SOLUTION
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
penaltyValue    = 1e3
leftBCID    = model.addBoundaryCondition(   DirichletBoundary(  0, 
                                                                lambda t: 0.0, 
                                                                penaltyValue=penaltyValue   ) )

rightBCID   = model.addBoundaryCondition(   NeumannBoundary(    nElements*polynomialOrder,
                                                                referenceControl) )

# Solve
initialSolution     = np.zeros( model.size )
referenceTimeSeries = solveLinearHeat1D(    time, 
                                            initialSolution, 
                                            model, 
                                            theta=finiteDifferenceImplicity )

# ---------------------------------------------------------
# OPTIMIZATION
# ---------------------------------------------------------
# Allocate
functionalValues    = np.zeros( numberOfAdjointIterations )
controls            = np.zeros( (numberOfAdjointIterations, len(time)) )

# Set initial control
#u                   = 0.1*np.random.rand( len(time) )
u                   = 0.5*np.ones( len(time) )

def controlFunction( t, control ):
    index = np.abs( time-t ).argmin()
    return control[index]

# Optimization loop
for i in range(numberOfAdjointIterations):

    # Set control
    model.boundaries[rightBCID].value   = lambda t: controlFunction( t, u )
    model.updateTime( time[0] )

    # Compute solution for the current control
    timeSeries  = solveLinearHeat1D(    time, 
                                        initialSolution, 
                                        model, 
                                        theta=finiteDifferenceImplicity )

    #########################################################################
    # Solve the stationary adjoint, and use the solution as the initial one
    initialAdjointSolution=None
    sol, info   = linalg.gmres( model.stiffness,
                                timeSeries[-1] - referenceTimeSeries[-1],
                                x0=np.random.rand(model.size),
                                atol=1e-12,
                                maxiter=np.max((5*model.size,100)) )
    initialAdjointSolution=sol
    #########################################################################

    # Compute adjoint solution
    adjointTimeSeries   = solveAdjointLinearHeat1D( time,
                                                    timeSeries - referenceTimeSeries,
                                                    model,
                                                    theta=finiteDifferenceImplicity,
                                                    initialAdjointSolution=initialAdjointSolution   )

    # Compute projections and update control
    projections         = -1.0/regularization * np.asarray([ 
                                model.boundaries[rightBCID].value(t) * adjointTimeSeries[tIndex][-1] for tIndex, t in enumerate(time)
                                ])
    #projections         = np.asarray([ np.min((0.04,np.max((0.0,projection)))) for projection in projections ])
    u                   += projections

    # Get functional integrated in space (for every time step)
    functionalValue = [ squaredSolutionErrorFunctional( solution,
                                                        referenceSolution,
                                                        model   )
                        for solution, referenceSolution in zip(timeSeries, referenceTimeSeries) ]
    
    # Integrate functional in time
    functionalValue = np.trapz( functionalValue,
                                x=time  )

    # Save intermediate results
    controls[i]         = u
    functionalValues[i] = functionalValue

    # Print detauls
    message     = "Iteration\t\t:%i" % i
    message     += "\nNorm of control\t\t:%.3f" % np.linalg.norm( u )
    message     += "\nFunctional value\t:%.4f" % functionalValue
    message     += "\n"
    print( message )




# ---------------------------------------------------------
# POSTPROCESSING
# ---------------------------------------------------------
# Find best control
uIndex  = functionalValues.argmin()
u       = controls[uIndex]

# Compute solution with best control
model.boundaries[rightBCID].value   = lambda t: controlFunction( t, u )
timeSeries = solveLinearHeat1D( time, 
                                initialSolution, 
                                model, 
                                theta=finiteDifferenceImplicity )


# Output and graphics
print( "\nFinal control:\t" + str(u) + "\nin iteration " + str(uIndex) )

samples             = np.linspace( 0, length, num=numberOfSamples )
animateTimeSeries(  time,
                    samples,
                    np.asarray((referenceTimeSeries, timeSeries)),
                    model,
                    speed=0.05,
                    ylim=(-0.1,1.0) )

#plt.loglog( functionalValues, ".-" )
plt.plot( functionalValues, ".-" )
plt.yscale( "log" )
plt.xlabel( "# Iteration" )
plt.ylabel( "Functional value" )
plt.show()