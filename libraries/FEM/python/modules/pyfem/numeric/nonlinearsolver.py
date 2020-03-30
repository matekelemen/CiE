# --- Internal Imports ---
from .solver import solveLinearSystem

# --- Python Imports ---
import numpy as np

# ---------------------------------------------------------
def stationaryLoadControl(  model,
                            initialSolution,
                            loadFunctional=None,
                            boundaryFunctional=None,
                            maxIncrements=5,
                            maxCorrections=5,
                            tolerance=1e-8,
                            verbose=True,
                            axes=None ):
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
    if loadFunctional is None:
        loadFunctional = lambda controlParameter: lambda x: 0.0

    if boundaryFunctional is None:
        boundaryFunctional = lambda controlParameter: None

    previousLoad = None

    # ---------------------------------------------------------
    # Increment loop
    for incrementIndex, control in enumerate(np.linspace( 0.0, 1.0, num=maxIncrements )):
        # Wipe model
        model.resetMatrices()

        # Set load function
        for element in model.elements:
            element.load = loadFunctional(control)

        # Initialize structural matrices
        u = initialSolution
        model.integrate( lambda x: model.sample(u, x) )

        # Apply boundaries
        boundaryFunctional(control)
        for boundary in model.boundaries:
            model.applyBoundaryCondition( boundary )

        # Check if first run (initialization)
        if previousLoad is None:
            previousLoad = model.load

        if verbose:
            print( "Increment# " + str(incrementIndex) + " " + "-"*(35-11-len(str(incrementIndex))-1) )

        # Predict
        u           += solveLinearSystem( model.stiffness, model.load - previousLoad )

        # Compute prediction residual
        residual    = model.stiffness.dot(u) -  model.load
        if verbose:
            print( "Prediction residual\t: %.3E" % np.linalg.norm( residual ) )

        # Correction loop
        for correctionIndex in range(maxCorrections):
            # Correct
            u           += solveLinearSystem( model.stiffness, -residual )

            # Update residual and check termination criterion
            residual    = model.stiffness.dot(u) - model.load
            resNorm     = np.linalg.norm(residual)
            if verbose:
                print( "Corrected residual\t: %.3E" % resNorm )
            if resNorm < tolerance:
                break

        # Update load
        previousLoad = model.load

        # Plot if requested
        if axes is not None:
            axes.plot( np.linspace( 0, 1, num=100 ), model.sample( u, np.linspace( 0, 1, num=100 ) ), '.-' )

    # Decorate plot if requested
    if axes is not None:
        axes.legend( [ "Control parameter = %.2f" % l for l in np.linspace( 0.0, 1.0, num=maxIncrements ) ] )
        axes.set_xlabel( "x [m]" )
        axes.set_ylabel( "T [C]" )
    
    return u