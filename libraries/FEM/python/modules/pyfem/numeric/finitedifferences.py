# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg

# --- Internal Imports ---
from .solver import solveLinearSystem
from .nonlinearsolver import transientFixedPointIteration

# ---------------------------------------------------------
def solveLinearHeat1D(  time, 
                        initialSolution, 
                        model,
                        theta=0.5   ):
    '''
    Arguments:
        time            : discretized time domain
        initialSolution : solution at t=0
        model           : TransientFEModel with mass, stiffness, load, and loadFunction
        theta=0.5       : implicitness
    '''
    # Initialize
    timeSeries     = np.zeros( (len(time), len(initialSolution)) )
    timeSeries[0]  = initialSolution.copy()

    model.updateTime( time[0] )
    nextLoadVector  = model.load.copy()

    # Define matrices 
    # (save computation of frequently used matrices if time discretization is uniform)
    timeStep    = time[1]-time[0]
    _LHSMatrix  = 1.0/timeStep*model.mass + theta*model.stiffness
    _RHSMatrix  = 1.0/timeStep*model.mass - (1.0-theta)*model.stiffness

    def LHSMatrix( DT ):
        if np.abs(timeStep - DT)<1e-15:
            return _LHSMatrix
        else:
            return 1.0/DT*model.mass + theta*model.stiffness

    def RHSMatrix( DT ):
        if np.abs(timeStep - DT)<1e-15:
            return _RHSMatrix
        else:
            return 1.0/DT*model.mass - (1.0-theta)*model.stiffness

    # Step
    for k in range(len(time)-1):
        # Time step
        dt  = time[k+1] - time[k]

        # Get load vectors
        model.updateTime( time[k+1] )
        currentLoadVector   = nextLoadVector.copy()
        nextLoadVector      = model.load.copy()

        # Step
        sol = solveLinearSystem(    LHSMatrix(dt),
                                    RHSMatrix(dt).dot(timeSeries[k])    \
                                        + theta*nextLoadVector          \
                                        + (1.0-theta)*currentLoadVector  )

        timeSeries[k+1] = sol

    return timeSeries



def solveAdjointLinearHeat1D(   time, 
                                adjointRHS,
                                model,
                                theta=0.5,
                                initialAdjointSolution=None ):
    '''
    Arguments:
        time                        : discretized time domain
        adjointRHS                  : FESolution - referenceFESolution
        model                       : TransientFEModel with mass, stiffness
        theta=0.5                   : implicitness
        initialAdjointSolution=None : zeros by default
    '''
    # Initialize
    timeSeries      = np.zeros( (len(time), model.size) )

    if initialAdjointSolution is not None:
        timeSeries[-1]  = initialAdjointSolution.copy()

    # Define matrices 
    # (save computation of frequently used matrices if time discretization is equidistant)
    timeStep    = time[1]-time[0]
    _LHSMatrix  = 1.0/timeStep*model.mass + theta*model.stiffness
    _RHSMatrix  = 1.0/timeStep*model.mass - (1.0-theta)*model.stiffness

    def LHSMatrix( DT ):
        if np.abs(timeStep - DT)<1e-15:
            return _LHSMatrix
        else:
            return 1.0/DT*model.mass + theta*model.stiffness

    def RHSMatrix( DT ):
        if np.abs(timeStep - DT)<1e-15:
            return _RHSMatrix
        else:
            return 1.0/DT*model.mass - (1.0 - theta) * model.stiffness

    # Step
    for k in range( len(time)-1, 0, -1 ):
        # Time step
        dt      = time[k] - time[k-1]

        # Step
        sol = solveLinearSystem(    LHSMatrix(dt),
                                    RHSMatrix(dt).dot( timeSeries[k] )  \
                                        + theta * adjointRHS[k-1]       \
                                        + (1.0-theta) * adjointRHS[k]   )

        # Neglect transient adjoint system and solve the stationary one at each time step 
        #sol     = solveLinearSystem(    model.stiffness,
        #                                adjointRHS[k]    )

        # Update time series
        timeSeries[k-1] = sol

    return timeSeries



def solveNonlinearHeat1D(   time,
                            initialSolution,
                            model,
                            theta=0.5,
                            nonlinearSolver=transientFixedPointIteration,
                            **kwargs   ):
    '''
    Arguments:
        time            : discretized time domain
        initialSolution : solution at t=0
        model           : NonlinearTransientFEModel with at least mass, stiffness, load, and loadFunction
        theta           : implicitness
        nonlinearSolver : either a variant of the Newton method or a fixed point iteration
        kwargs          : keyword arguments of the nonlinearSolver
    '''
    # Initialize
    timeSeries          = np.zeros( (len(time), len(initialSolution)) )
    timeSeries[0]       = initialSolution.copy()

    model.updateTime(   time[0],
                        timeSeries[0],
                        geometricStiffness=False    )
    previousStiffness   = None
    previousMass        = None
    previousLoad        = None

    # Step through 
    for k in range(1, len(time)):
        # Cache previous state
        previousStiffness   = model.stiffness.copy()
        previousMass        = model.mass.copy()
        previousLoad        = model.load.copy()

        # Advance state
        model.updateTime(   time[k],
                            timeSeries[k-1],
                            stiffness=False,
                            mass=False,
                            geometricStiffness=False    )
        
        # Delegate solving to the nonlinear solver
        timeSeries[k]       = nonlinearSolver(  model,
                                                timeSeries[k-1],
                                                previousStiffness,
                                                previousMass,
                                                previousLoad,
                                                time[k] - time[k-1],
                                                **kwargs    )

    return timeSeries