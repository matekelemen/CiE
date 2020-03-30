# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
import scipy
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import NonlinearHeatElement1D
from pyfem.discretization import FEModel
from pyfem.discretization import DirichletBoundary, NeumannBoundary
from pyfem.numeric import solveLinearSystem

# ---------------------------------------------------------
# Geometry and material
length              = 1.0
capacity            = lambda u: 1.0
conductivity        = lambda u: 1.0 + 9.0 * np.exp( -(u-0.5)**2 / 0.005 )

# Load
load                = lambda x: 0.0
boundaryFlux        = 2.0
fluxIncrement       = 0.5

# Discretization
nElements           = 10
polynomialOrder     = 2

# Integration
integrationOrder    = 2*2*polynomialOrder + 1


# ---------------------------------------------------------
# General initialization
fluxIncrements  = fluxIncrement * np.ones( int(boundaryFlux / fluxIncrement) )

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

rightBCID   = model.addBoundaryCondition(   NeumannBoundary(    nElements*polynomialOrder,
                                                                0.0) )

# Initial solution
u               = np.zeros( model.size )
previousLoad    = np.zeros( model.size )
axes[1].plot( samples, model.sample( u, samples ), '.-' )

for incrementIndex, loadIncrement in enumerate( fluxIncrements ):
    # Print increment separator
    print( "Increment# " + str(incrementIndex+1) + " " + "-"*(35-11-len(str(incrementIndex+1))-1) )
    
    # Reset model
    model.resetMatrices()

    # Integrate with the current solution
    model.integrate( lambda x: model.sample(u, x) )

    # Apply boundary conditions and load
    model.boundaries[rightBCID].value += loadIncrement
    for boundary in model.boundaries:
        model.applyBoundaryCondition( boundary )

    # Prediction
    increment       = solveLinearSystem( model.stiffness, model.load - previousLoad )
    u               += increment
    previousLoad    = model.load

    # Compute prediction residual
    residual    = model.stiffness.dot(u) -  model.load
    print( "Prediction residual\t: %.3E" % np.linalg.norm( residual ) )

    # Correction
    for i in range(5):
        # Compute correction
        correction  = solveLinearSystem( model.stiffness, -residual )
        u           += correction

        # Update residual and check termination
        residual    = model.stiffness.dot(u) -  model.load
        resNorm     = np.linalg.norm( residual )
        print( "Corrected residual\t: %.3E" % resNorm )
        if resNorm < 1e-8:
            break

    # Plot solution of current increment
    axes[1].plot( samples, model.sample( u, samples ), '.-' )


# Plot conductivity
samples = np.linspace( 0.0, np.max(model.sample( u, samples )), num=len(samples) )
axes[0].plot( samples, [conductivity(temp) for temp in samples] )
axes[0].set_xlabel( "T [C]" )
axes[0].set_ylabel( r'$\kappa$' + " [W/K]" )

# Plot incremental solutions
axes[1].set_xlabel( "x [m]" )
axes[1].set_ylabel( "T [C]" )
axes[1].legend( ["Flux = %.2f" % 0.0] + [ "Flux = %.2f" % flux for flux in np.cumsum(fluxIncrements) ] )

plt.show()