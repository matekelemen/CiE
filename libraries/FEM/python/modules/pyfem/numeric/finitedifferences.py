# --- Python Imports ---
import numpy as np
import scipy.sparse.linalg as linalg

# ---------------------------------------------------------
def separableFirstOrderThetaScheme( time, 
                                    initialSolution, 
                                    model,
                                    load,
                                    theta=0.5 ):
    '''
    Implicit theta-scheme. Solves the following problem:
    coefficient * u{i+1} = 

    Arguments:
        time            : discretized time domain
        initialSolution : solution at t=0
        model           : FEModel with mass, stiffness, and load,
        load            : function of time and space
        theta=0.5       : implicitness
    '''
    # Initialize
    timeHistory     = np.zeros( (len(time), len(initialSolution)) )
    timeHistory[0]  = initialSolution

    model.updateLoad( lambda x: load(time[0], x) )
    nextLoadVector  = model.load

    # Step
    for k in range(len(time)-1):
        # Time step
        dt  = time[k+1] - time[k]

        # Get load vectors
        model.updateLoad( lambda x: load( time[k+1], x ) )
        currentLoadVector   = nextLoadVector
        nextLoadVector      = model.load

        # Step
        sol, info   = linalg.gmres( 1.0/dt*model.mass + theta*model.stiffness,
                                    ( 1.0/dt*model.mass - (1.0-theta)*model.stiffness).dot(timeHistory[k])  \
                                        + theta*nextLoadVector                                              \
                                        + (1.0-theta)*currentLoadVector,
                                    x0=np.random.rand(model.size),
                                    atol=1e-12,
                                    maxiter=np.max((5*model.size,100)) )

        if info < 0:
            raise RuntimeError( "Linear solver error!" )
        elif info>0:
            print( "Solution failed to converge after " + str(info) + " steps (step " + str(k) + ")" )

        timeHistory[k+1] = sol

    return timeHistory