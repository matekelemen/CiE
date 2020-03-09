# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg

# ---------------------------------------------------------
def solveLinearHeat1D(  time, 
                        initialSolution, 
                        model,
                        theta=0.5,
                        equidistantTime=False ):
    '''
    Implicit theta-scheme. Solves the following problem:
    coefficient * u{i+1} = 

    Arguments:
        time            : discretized time domain
        initialSolution : solution at t=0
        model           : TransientFEModel with mass, stiffness, load, and loadFunction
        theta=0.5       : implicitness
    '''
    # Initialize
    timeSeries     = np.zeros( (len(time), len(initialSolution)) )
    timeSeries[0]  = initialSolution

    model.updateLoad( lambda x: model.loadFunction(time[0], x) )
    nextLoadVector  = model.load

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
            return 1.0/DT*model.mass - (1.0-theta)*model.stiffness

    # Step
    for k in range(len(time)-1):
        # Time step
        dt  = time[k+1] - time[k]

        # Get load vectors
        model.updateTime( time[k+1] )
        currentLoadVector   = nextLoadVector
        nextLoadVector      = model.load

        # Step
        sol, info   = linalg.gmres( LHSMatrix(dt),
                                    RHSMatrix(dt).dot(timeSeries[k])   \
                                        + theta*nextLoadVector                                              \
                                        + (1.0-theta)*currentLoadVector,
                                    x0=np.random.rand(model.size),
                                    atol=1e-12,
                                    maxiter=np.max((5*model.size,100)) )

        if info < 0:
            raise RuntimeError( "Linear solver error!" )
        elif info>0:
            print( "Solution failed to converge after " + str(info) + " steps (step " + str(k) + ")" )

        timeSeries[k+1] = sol

    return timeSeries



def solveAdjointLinearHeat1D(   time, 
                                adjointRHS,
                                model,
                                theta=0.5,
                                initialAdjointSolution=None ):
    '''
    Implicit theta-scheme. Solves the following problem:
    coefficient * u{i+1} = 

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
        timeSeries[-1]  = initialAdjointSolution

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
        dt  = time[k] - time[k-1]

        # Step
        sol, info   = linalg.gmres( LHSMatrix(dt),
                                    RHSMatrix(dt).dot( timeSeries[k] )  \
                                        + theta * adjointRHS[k-1]                                                 \
                                        + (1.0-theta) * adjointRHS[k],
                                    x0=np.random.rand(model.size),
                                    atol=1e-12,
                                    maxiter=np.max((5*model.size,100)) )

        # Check convergence
        if info < 0:
            raise RuntimeError( "Linear solver error!" )
        elif info>0:
            print( "Solution failed to converge after " + str(info) + " steps (step " + str(k) + ")" )

        # Update time series
        timeSeries[k-1] = sol

    return timeSeries