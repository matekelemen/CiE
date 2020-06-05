# --- Discretization Imports ---
from pyfem.discretization import NonlinearTransientFEModel

# --- Python Imports ---
import numpy as np

# ---------------------------------------------------------
class AdjointModel(NonlinearTransientFEModel):
    '''
    Nonlinear adjoint model attached to a NonlinearTransientFEModel
    '''
    def __init__( self, model, time, referenceTimeHistory, forwardTimeHistory, *args, **kwargs ):
        NonlinearTransientFEModel.__init__( self, *args, **kwargs )

        # Check model type
        assert type(model) is NonlinearTransientFEModel, "Invalid model type!"

        self.femodel            = model
        self.time               = time
        self.referenceSolution  = referenceTimeHistory
        self.forwardSolution    = forwardTimeHistory

        self._loadFunction      = self.adjointLoadFunction
        
        # Transform model members
        for element in model.elements:
            self.elements.append(element.copy())

    
    def makeSampleFunctor( self, coefficients ):
        return lambda x: self.femodel.sample( coefficients, x )

    
    def adjointLoadFunction( self, t, x ):
        timeIndex = next( (index for index,time in enumerate(self.time) if np.ans(time-t)>1e-16), -1 )
        assert timeIndex > 0, "Time index not found for t=%.3f" % t

        referenceField  = self.makeSampleFunctor( self.referenceSolution[timeIndex] )
        forwardField    = self.makeSampleFunctor( self.forwardSolution[timeIndex] )
        return lambda t,x: referenceField(x) - forwardField(x)