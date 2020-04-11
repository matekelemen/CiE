# --- Internal Imports ---
from .solver import solveLinearSystem

# --- Python Imports ---
import numpy as np
import scipy.sparse as sparse

# ---------------------------------------------------------
def newtonIteration(    targetSystem,
                        initialSolution,
                        tolerance=1e-5,
                        maxIterations=10,
                        verbose=False,
                        convergencePlot=None    ):
    '''
    Attempts to find a root of a target function, using 1st order linearization.
    The target function must return a 1D numpy array, while the target derivative
    must return a matching nonsingular square numpy ndarray. Both the function and
    derivative must expect the state (1D numpy array) as argument (and no other
    positional arguments).
    '''
    # Initialize
    solution                    = initialSolution.copy()
    functionValue, derivative   = targetSystem(solution)
    iterationCounter            = 1
    targetNorm                  = np.linalg.norm(functionValue)

    if verbose:
        print( "Initial target\t: %.3E" % targetNorm )
    if convergencePlot is not None:
        convergencePlot(targetNorm)

    # Loop
    while True:
        # Check if iteration limit exceeded
        if iterationCounter >= maxIterations:
            print( "Newton iteration failed to converge after %i iterations" % iterationCounter )
            break

        # Step
        solution                    -= solveLinearSystem( derivative, functionValue )
        functionValue, derivative   = targetSystem(solution)

        # Check convergence
        targetNorm                  = np.linalg.norm(functionValue)
        if verbose:
            print( "Target norm\t: %.3E" % targetNorm )
        if convergencePlot is not None:
            convergencePlot(targetNorm)
        if targetNorm < tolerance:
            break

        iterationCounter            += 1
    
    return solution



def reintegrate(    model, 
                    loadFactor, 
                    solution,
                    stiffness=True,
                    load=True,
                    mass=True,
                    geometricStiffness=True ):
    '''
    Function that does all the necessary operations for updating 
    the model to the current load factor and solution
    '''
    # Set up arguments for stationary or transient models
    keywordArgs = { "stiffness"             : stiffness,
                    "load"                  : load,
                    "geometricStiffness"    : geometricStiffness }

    if hasattr( model, "mass" ):
        keywordArgs["mass"] = mass

    # Wipe matrices and set solution function
    model.resetMatrices( **keywordArgs )
    solutionFunction = lambda x: model.sample(solution, x)

    # Compute structural matrices
    model.integrate(    solutionFunction, 
                        solution, 
                        **keywordArgs )

    # Apply boundaries
    for boundary in model.boundaries:
        model.applyBoundaryCondition( boundary )



def stationaryLoadControl(  model,
                            initialSolution,
                            loadFactors=None,
                            maxCorrections=10,
                            tolerance=1e-5,
                            verbose=True,
                            axes=None,
                            convergencePlot=None ):
    '''
    Solves a nonlinear FEModel using a load controlled predictor and a constant load corrector
    '''
    # ---------------------------------------------------------
    # Initialize arguments
    u               = initialSolution.copy()

    if loadFactors is None:
        loadFactors     = np.linspace( 0.0, 1.0, num=5+1 )

    if convergencePlot is not None:
        convergencePlot.start()

    # Define inputs for the Newton iteration
    def updateSystem(solution, loadFactor):
        reintegrate(    model,
                        loadFactor,
                        solution,
                        mass=False)
        residual            = model.stiffness.dot(solution) - loadFactor*model.load
        tangentStiffness    = model.stiffness + model.geometricStiffness
        return residual, tangentStiffness

    # Initialize structural matrices
    reintegrate(    model,
                    loadFactors[0],
                    initialSolution,
                    mass=False)
    
    # ---------------------------------------------------------
    # Increment loop
    for incrementIndex, control in enumerate(loadFactors):

        if incrementIndex == 0:
            continue

        # Print iteration header
        if verbose:
            print( "\nIncrement# " + str(incrementIndex) + " " + "-"*(35-11-len(str(incrementIndex))-1) )

        # Predict
        controlIncrement    = control-loadFactors[incrementIndex-1]
        u                   += solveLinearSystem( model.stiffness + model.geometricStiffness, controlIncrement * model.load )
        #uMid                = u + 0.5 * solveLinearSystem( model.stiffness + model.geometricStiffness, controlIncrement * model.load )
        #reintegrate(    model, 
        #                control, 
        #                uMid,
        #                mass=False )
        #u                   += solveLinearSystem( model.stiffness + model.geometricStiffness, controlIncrement * model.load )

        # Correction loop
        u           = newtonIteration(  lambda solution: updateSystem(solution, control),
                                        u,
                                        tolerance=tolerance,
                                        maxIterations=maxCorrections,
                                        verbose=True,
                                        convergencePlot=convergencePlot)

        # Plot if requested
        if axes is not None:
            axes.plot( np.linspace( 0, 1, num=100 ), model.sample( u, np.linspace( 0, 1, num=100 ) ) )

    # ---------------------------------------------------------
    # Decorate plot if requested
    if axes is not None:
        axes.legend( [ "Control parameter = %.2f" % l for l in loadFactors[1:] ] )
        axes.set_xlabel( "x [m]" )
        axes.set_ylabel( "T [C]" )
        axes.set_title( "Temperature Field" )

    if convergencePlot is not None:
        convergencePlot.finish()
    
    return u




def stationaryFixedPointIteration(  model,
                                    initialSolution,
                                    loadFactors=None,
                                    maxCorrections=10,
                                    tolerance=1e-5,
                                    verbose=True,
                                    axes=None,
                                    convergencePlot=None ):
    '''
    Solves a nonlinear FEModel using fixed point iterations
    '''
    # ---------------------------------------------------------
    # Initialize arguments
    u               = initialSolution

    if loadFactors is None:
        loadFactors     = np.linspace( 0.0, 1.0, num=5+1 )

    if convergencePlot is not None:
        convergencePlot.start()
    
    # ---------------------------------------------------------
    # Increment loop
    for incrementIndex, control in enumerate(loadFactors):

        # Print iteration header
        if verbose:
            print( "\nIncrement# " + str(incrementIndex) + " " + "-"*(35-11-len(str(incrementIndex))-1) )

        # Check if first run (initialization)
        if incrementIndex == 0:
            reintegrate(    model, 
                            loadFactors[0], 
                            u,
                            mass=False,
                            geometricStiffness=False )
            continue

        # Correction loop
        for correctionIndex in range(maxCorrections):
            # Correct
            u           = solveLinearSystem( model.stiffness, control * model.load )

            # Update residual and check termination criterion
            reintegrate(    model, 
                            control, 
                            u,
                            mass=False,
                            geometricStiffness=False )
            residual    = model.stiffness.dot(u) - control*model.load
            resNorm     = np.linalg.norm(residual)
            if verbose:
                print( "Current residual\t: %.3E" % resNorm )
            if convergencePlot is not None:
                convergencePlot( resNorm )
            if resNorm < tolerance:
                break
            elif correctionIndex == maxCorrections-1:
                print( "Warning: corrector failed to converge within the specified tolerance" )

        # Plot if requested
        if axes is not None:
            axes.plot( np.linspace( 0, 1, num=100 ), model.sample( u, np.linspace( 0, 1, num=100 ) ) )

    # ---------------------------------------------------------
    # Decorate plot if requested
    if axes is not None:
        axes.legend( [ "Control parameter = %.2f" % l for l in loadFactors[1:] ] )
        axes.set_xlabel( "x [m]" )
        axes.set_ylabel( "T [C]" )
        axes.set_title( "Temperature Field" )

    if convergencePlot is not None:
        convergencePlot.finish()
    
    return u




def transientFixedPointIteration(   model,
                                    initialSolution,
                                    initialStiffness,
                                    initialMass,
                                    initialLoad,
                                    dt,
                                    loadFactors=None,
                                    maxCorrections=10,
                                    tolerance=1e-5,
                                    theta=0.5,
                                    verbose=True,
                                    convergencePlot=None ):
    '''
    Solves a transient nonlinear FEModel using fixed point iterations
    ( Arguments are copied internally )
    '''
    # ---------------------------------------------------------
    # Initialize arguments
    u                   = initialSolution.copy()
    DT                  = 1.0/dt * sparse.eye(model.size)
    resNorm             = None

    if loadFactors is None:
        loadFactors     = [1.0]

    if convergencePlot is not None:
        convergencePlot.start()

    # Initialize system components
    initialState        =   (1.0-theta) * sparse.linalg.inv(initialMass).dot( initialLoad - initialStiffness.dot(u) ) \
                            + DT.dot(u)
    
    # ---------------------------------------------------------
    # Increment loop
    for incrementIndex, control in enumerate(loadFactors):

        # Print iteration header
        if verbose:
            print( "\nIncrement# " + str(incrementIndex) + " " + "-"*(35-11-len(str(incrementIndex))-1) )

        # Correction loop
        for correctionIndex in range(maxCorrections):
            # Update residual
            reintegrate(    model, 
                            control, 
                            u,
                            geometricStiffness=False )

            massInverse     = sparse.linalg.inv(model.mass)
            currentState    = theta * massInverse.dot(model.stiffness) + DT
            currentLoad     = theta * massInverse.dot(control * model.load) + initialState
            
            residual        = currentState.dot(u) - currentLoad
            resNorm         = np.linalg.norm(residual)
            
            # Check termination criterion and update output streams
            if verbose:
                print( "Current residual\t: %.3E" % resNorm )
            if convergencePlot is not None:
                convergencePlot( resNorm )
            if resNorm < tolerance:
                break

            # Correct
            u           = solveLinearSystem(    currentState,
                                                currentLoad )

        # Compute final residual
        if resNorm > tolerance:
            reintegrate(    model, 
                            loadFactors[-1], 
                            u,
                            geometricStiffness=False )
            massInverse     = sparse.linalg.inv(model.mass)
            currentState    = theta * massInverse.dot(model.stiffness) + DT
            currentLoad     = theta * massInverse.dot(loadFactors[-1] * model.load) + initialState
            residual        = currentState.dot(u) - currentLoad
            resNorm         = np.linalg.norm(residual)

            if verbose:
                print( "Final residual\t\t: %.3E" % resNorm )

        if resNorm > tolerance:
            print( "Nonlinear solver failed to correct increment" )

    # ---------------------------------------------------------
    #uDot    = (u-initialSolution)/dt
    #eq      = model.mass.dot(uDot) + model.stiffness.dot(u) - model.load
    #print( "Eq residual\t\t: %.3E" % np.linalg.norm(eq) )

    if resNorm > tolerance:
        raise RuntimeError( "Nonlinear solver failed to converge" )

    if convergencePlot is not None:
        convergencePlot.finish()
    
    return u