import numpy as np
from timeit import default_timer
from vispy import app

class SimpleLight:
    # Simple spot light
    def __init__(self,parent,lightPos=[0.0,0.0,1.0],lightColor=[1.0,1.0,1.0]):
        self._parent        = parent
        self._lightPos      = lightPos
        self._lightColor    = lightColor
        self.update()

    def update(self,lightPos=None,lightColor=None):
        if lightPos is not None:
            self._lightPos      = lightPos
        if lightColor is not None:
            self._lightColor    = lightColor
        self._parent.update()


class TimedSpotLight(SimpleLight):
    def __init__(self,parent,pathFunction=None,*args,**kwargs):
        SimpleLight.__init__(self,parent,**kwargs)

        if pathFunction is None:
            pathFunction = defaultPathFunction
        self._pathFunction = pathFunction

        self._t0    = default_timer()
        self._timer = app.Timer('auto', connect=self.on_timer, start='true')

    
    def on_timer(self,event):
        self.update(lightPos=self._pathFunction( default_timer()-self._t0 ))

    



def defaultPathFunction(t):
    return np.array([ np.cos(t), np.sin(t), 1.0 ],dtype=np.float32)