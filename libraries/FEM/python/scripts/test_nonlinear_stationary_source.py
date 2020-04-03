# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg
from matplotlib import pyplot as plt

# --- Internal Imports ---
from pyfem.discretization import IntegratedHierarchicBasisFunctions
from pyfem.discretization import NonlinearHeatElement1D
from pyfem.discretization import NonlinearFEModel
from pyfem.discretization import DirichletBoundary
from pyfem.numeric import stationaryFixedPointIteration as nonlinearSolver
from pyfem.postprocessing import ConvergencePlot

'''
Standard stationary setup with zero Dirichlet conditions on both ends. The bar is 
loaded with a half sine wave shaped source. One twist is that the material is 
nonlinear (sharp gaussian hump in capacity centered at u=0.5). The other twist is 
that the calculation is carried out twice:

    1)  incrementing is broken down into two parts: 
        increment to half load, then use the result and increment from half to full load.

    2)  increment from naught to full load in one function call.

Both cases use the same number of increments (unless numberOfIncrements is odd).
Obviously, the results should be identical (with minor numerical inaccuracies).
'''

# ---------------------------------------------------------
# Geometry and material
length              = 1.0
capacity            = lambda u: 1.0
dCapacity           = lambda u: 0.0
#conductivity        = lambda u: 1.0
#dConductivity       = lambda u: 0.0
conductivity        = lambda u: 1.0 + 9.0 * np.exp( -(u-0.5)**2 / 0.005 )
dConductivity       = lambda u: 9.0 * np.exp( -(u-0.5)**2 / 0.005 ) * (2.0/0.005)*(0.5-u)

# Load
load                = lambda x: 20.0 * np.sin(np.pi/length*x)

# Discretization
nElements           = 15
polynomialOrder     = 3
penaltyValue        = 1e10

# Integration
integrationOrder    = 2 * (2*polynomialOrder + 1)

# Iteration
numberOfIncrements  = 4
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

# Boundary conditions
leftBCID    = model.addBoundaryCondition(   DirichletBoundary(  0, 
                                                                0.0,
                                                                penaltyValue=penaltyValue ))

rightBCID   = model.addBoundaryCondition(   DirichletBoundary(  nElements*polynomialOrder,
                                                                0.0,
                                                                penaltyValue=penaltyValue) )

# ---------------------------------------------------------
# Solve in 2 increment iterations, round 1
loadFactors     = np.linspace( 0.0, 0.5, num=int(numberOfIncrements/2)+1 )

# Solve
convergencePlot = ConvergencePlot()
u = nonlinearSolver(    model,
                        np.zeros(model.size),
                        loadFactors=loadFactors,
                        maxCorrections=numberOfCorrections,
                        tolerance=tolerance,
                        verbose=True,
                        axes=None,
                        convergencePlot=convergencePlot   )

# Solve in 2 increment iterations, round 2
loadFactors     = np.linspace( 0.5, 1.0, num=int(numberOfIncrements/2)+1 )

# Solve
u = nonlinearSolver(    model,
                        u,
                        loadFactors=loadFactors,
                        maxCorrections=numberOfCorrections,
                        tolerance=tolerance,
                        verbose=True,
                        axes=None,
                        convergencePlot=convergencePlot   )
convergencePlot.reset()

u2 = u.copy()
# ---------------------------------------------------------
# Solve in 1 increment iteration
loadFactors     = np.linspace( 0.0, 1.0, num=numberOfIncrements+1 )

# Solve
convergencePlot.reset()
u = nonlinearSolver(    model,
                        np.zeros(model.size),
                        loadFactors=loadFactors,
                        maxCorrections=numberOfCorrections,
                        tolerance=tolerance,
                        verbose=True,
                        axes=None,
                        convergencePlot=convergencePlot   )
# ---------------------------------------------------------
# Print solution difference
print( "\nNorm of solution difference\t: %.3E" % np.linalg.norm(u-u2) )

# Plot conductivity
samples = np.linspace( 0.0, np.max(model.sample( u, samples )), num=len(samples) )
axes[0].plot( samples, [conductivity(temp) for temp in samples] )
axes[0].set_xlabel( "T [C]" )
axes[0].set_ylabel( r'$\kappa$' + " [W/K]" )
axes[0].set_title( "Capacity(temperature)" )

# Plot solutions
samples = np.linspace( 0.0, length, num=100 )
axes[1].plot( samples, model.sample( u, samples ) )
axes[1].plot( samples, model.sample( u2, samples ), "--" )
axes[1].legend( ("Single pass", "Double pass") )
axes[1].set_xlabel( "x [m]" )
axes[1].set_ylabel( "T [C]" )
axes[1].set_title( "Temperature Field" )

plt.tight_layout()
fig.canvas.draw()
plt.show( block=True )