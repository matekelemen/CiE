# --- Discretization Imports ---
from pyfem.discretization import NonlinearTransientFEModel

# --- Python Imports ---


# ---------------------------------------------------------
class AdjointModel(NonlinearTransientFEModel):
    '''
    Nonlinear adjoint model attached to a NonlinearTransientFEModel
    '''
    def __init__( self, model, time, forwardTimeHistory, *args, **kwargs ):
        NonlinearTransientFEModel.__init__( self, *args, **kwargs )

        # Check model type
        assert type(model) is NonlinearTransientFEModel, "Invalid model type!"

        self.femodel            = model
        self.forwardSolution    = (time, forwardTimeHistory)
        
        # Transform model members
        for element in model.elements:
            self.elements.append(element.copy())

    
    def makeForwardSampleFunction( self, timeIndex ):
        return lambda x: self.femodel.sample( self.forwardSolution[1][timeIndex], x )