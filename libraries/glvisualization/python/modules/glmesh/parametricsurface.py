# --- Python imports ---
import numpy as np

# -----------------------------------------------------------
class ParametricSurface:
    def __init__(self, vertexFunctor, domain=((0.0,1.0),(0.0,1.0))):        
        self._functor   = vertexFunctor
        self._domain    = domain


    def evaluate(self, u, v):
        # Check arguments
        if min(u)<min(self._domain[0]) or max(u)>max(self._domain[0]):
            raise ValueError('Parameter u out of range')
        if min(v)<min(self._domain[1]) or max(v)>max(self._domain[1]):
            raise ValueError('Parameter v out of range')

        return np.array(
                [
                    [ [self._functor[componentIndex](U,V) for U in u] for V in v ] for componentIndex in (0,1,2)
                ]
                ,dtype=np.float32)