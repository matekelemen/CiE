# --- Python Imports ---
import numpy as np

# ---------------------------------------------------------
def SquaredErrorFunctional( solution, referenceSolution, model, loadFunction, time ):
    '''
    Integrates the squared difference of the specified solution and a reference solution over the time and space domain.
    '''
    # Initialize
    functionalValue = 0.0
    
    for t in time:
        pass