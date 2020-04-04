# --- Internal Imports ---
from .solver import solveLinearSystem

# --- Python Imports ---
import numpy as np
import scipy.sparse as sparse

# ---------------------------------------------------------
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
    # Wipe matrices and set solution function
    model.resetMatrices()
    solutionFunction = lambda x: model.sample(solution, x)

    # Compute structural matrices
    for element in model.elements:
        if stiffness:
            element.integrateStiffness( model.stiffness, solutionFunction, solution )

        if load:
            element.integrateLoad( model.load, solutionFunction, solution )

        if mass:
            element.integrateMass( model.mass, solutionFunction, solution )

        if geometricStiffness:
            element.integrateGeometricStiffness( model.geometricStiffness, solutionFunction, solution )

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
                            mass=False )
            continue

        # Predict
        controlIncrement    = control-loadFactors[incrementIndex-1]
        #u                   += solveLinearSystem( model.stiffness + model.geometricStiffness, controlIncrement * model.load )
        uMid                = u + 0.5 * solveLinearSystem( model.stiffness + model.geometricStiffness, controlIncrement * model.load )
        reintegrate(    model, 
                        control, 
                        uMid,
                        mass=False )
        u                   += solveLinearSystem( model.stiffness + model.geometricStiffness, controlIncrement * model.load )

        # Compute prediction residual
        reintegrate(    model, 
                        control, 
                        u,
                        mass=False )
        residual    = model.stiffness.dot(u) -  control*model.load
        resNorm     = np.linalg.norm( residual )
        if verbose:
            print( "Prediction residual\t: %.3E" % resNorm )
        if convergencePlot is not None:
            convergencePlot( resNorm )

        # Correction loop
        for correctionIndex in range(maxCorrections):
            # Correct
            u           += solveLinearSystem( model.stiffness + model.geometricStiffness, -residual )

            # Update residual and check termination criterion
            reintegrate(    model, 
                            control, 
                            u,
                            mass=False )
            residual    = model.stiffness.dot(u) - control*model.load
            resNorm     = np.linalg.norm(residual)
            if verbose:
                print( "Corrected residual\t: %.3E" % resNorm )
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
                                    axes=None,
                                    convergencePlot=None ):
    '''
    Solves a transient nonlinear FEModel using fixed point iterations
    ( Arguments are copied internally )
    '''
    # ---------------------------------------------------------
    # Initialize arguments
    u                   = initialSolution.copy()
    initialState        = None
    massInverse         = None
    currentState        = None
    currentLoad         = None
    dt                  = 1.0/dt * sparse.eye(model.size)

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
                            geometricStiffness=False )
            initialState    =   (1.0-theta) * sparse.linalg.inv(initialMass).dot( initialStiffness.dot(u) - initialLoad ) \
                                - dt.dot(u)
            massInverse     = sparse.linalg.inv(model.mass)
            currentState    = theta*massInverse*model.stiffness + dt
            currentLoad     = initialState - theta * massInverse.dot(control * model.load)
            continue

        # Correction loop
        for correctionIndex in range(maxCorrections):
            # Correct
            u           = solveLinearSystem(    currentState,
                                                currentLoad )

            # Update residual
            reintegrate(    model, 
                            control, 
                            u,
                            geometricStiffness=False )

            massInverse     = sparse.linalg.inv(model.mass)
            currentState    = theta*massInverse*model.stiffness + dt
            currentLoad     = initialState - theta * massInverse.dot(control * model.load)
            
            residual        = model.stiffness.dot(u) - control*model.load
            resNorm         = np.linalg.norm(residual)
            
            # Check termination criterion and update output streams
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