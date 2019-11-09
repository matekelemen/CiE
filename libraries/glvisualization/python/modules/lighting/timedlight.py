# --- Internal imports ---
from lighting.simplelight import SimpleLight

# --- Python imports ---
from timeit import default_timer
import numpy as np

# --- Vispy imports ---
from vispy import app

class TimedSpotLight(SimpleLight):
    def __init__(self, parent,
                                posFunctor=None,
                                colorFunctor=None,
                                *args,
                                **kwargs ):
        SimpleLight.__init__(self,parent,**kwargs)

        # Default functors
        if posFunctor is None:
            self._posFunctor    = defaultPosFunctor
        else:
            self._posFunctor    = posFunctor

        if colorFunctor is None:
            self._colorFunctor  = defaultColorFunctor
        else:
            self._colorFunctor  = colorFunctor

        # Timer setup
        self._t0    = default_timer()
        self._timer = app.Timer('auto', connect=self.on_timer, start='true')

    
    def on_timer(self, event):
        t = default_timer() - self._t0
        self.update( 
            pos=self._posFunctor( t ),
            color=self._colorFunctor( t )
            )

    



def defaultPosFunctor(t):
    return np.array([ 0.0, 0.0, 0.0 ],dtype=np.float32)


def defaultColorFunctor(t):
    return np.array( (0.5,0.5,0.5) , dtype=np.float32)