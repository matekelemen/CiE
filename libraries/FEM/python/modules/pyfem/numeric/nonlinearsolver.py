# --- Internal Imports ---
from .solver import solveLinearSystem

# --- Python Imports ---
import numpy as np

# ---------------------------------------------------------
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
    
    The structural matrices (and load vector) have to be reset at every load increment,
    so functions that set the load and boundaries at corresponing load increments must
    be provided. Load increments are defined as differences between subsequent control
    parameters (lambda). Control parameters always range from 0 in the initial state 
    to 1 in the final state.

    Arguments:
        model               : allocated FEModel with boundaries and a valid initial state
        initialSolution     : initial solution to begin incrementing from
        loadFunctional      : a function of the control parameter (lambda) that returns a load function
                                (source). If None, 0 source is assumed everywhere.
                                The load function gets applied to the FEModel before integration.
        boundaryFunctional  : a function of the control parameter (lambda) that manipulates the FEModel
                                by adjusting boundary condition values to match the desired boundary load.
                                If None, the existing boundaries in the FEModel will remain untouched.
                                This function gets called after integration.
        maxIncrements       : maximum number of increments to the control parameter
    '''
    # ---------------------------------------------------------
    # Initialize arguments
    u               = initialSolution

    if loadFactors is None:
        loadFactors     = np.linspace( 0.0, 1.0, num=5+1 )

    if convergencePlot is not None:
        convergencePlot.start()

    # Define function that does all the necessary operations
    # for updating the model to the current control parameter and solution
    def reintegrate( controlParameter, solution ):
        # Set to zero
        model.resetMatrices()

        # Compute structural matrices
        model.integrate( lambda x: model.sample(solution, x), solution )

        # Apply boundaries
        for boundary in model.boundaries:
            model.applyBoundaryCondition( boundary )

        #np.set_printoptions( precision=2, suppress=True )
        #print(dModel.stiffness.todense())
        #print(   np.linalg.det(   dModel.stiffness.todense()   )   )
        #np.set_printoptions()
    
    
    # ---------------------------------------------------------
    # Increment loop
    for incrementIndex, control in enumerate(loadFactors):
        if verbose:
            print( "\nIncrement# " + str(incrementIndex) + " " + "-"*(35-11-len(str(incrementIndex))-1) )

        # Check if first run (initialization)
        if incrementIndex == 0:
            reintegrate( loadFactors[0], u )
            continue

        # Predict
        controlIncrement    = control-loadFactors[incrementIndex-1]
        u                   += solveLinearSystem( model.stiffness, controlIncrement * model.load )
        #uMid                = 0.5 * solveLinearSystem( model.stiffness, controlIncrement * model.load )
        #reintegrate( control, uMid )
        #u                   += solveLinearSystem( model.stiffness, controlIncrement * model.load )

        # Compute prediction residual
        reintegrate( control, u )
        residual    = model.stiffness.dot(u) -  control*model.load
        resNorm     = np.linalg.norm( residual )
        if verbose:
            print( "Prediction residual\t: %.3E" % resNorm )
        if convergencePlot is not None:
            convergencePlot( resNorm )

        # Correction loop
        for correctionIndex in range(maxCorrections):
            # Correct
            u           += solveLinearSystem( model.stiffness, -residual )

            # Update residual and check termination criterion
            reintegrate( control, u )
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