# --- Internal Imports ---
from .solver import solveLinearSystem

# --- Python Imports ---
import numpy as np

# ---------------------------------------------------------
def stationaryLoadControl(  model,
                            initialSolution,
                            loadFunctional=None,
                            boundaryFunctional=None ):
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
    '''