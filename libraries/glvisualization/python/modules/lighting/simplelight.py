import numpy as np
from timeit import default_timer

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