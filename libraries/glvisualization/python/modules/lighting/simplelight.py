import numpy as np

class SimpleLight:
    # Simple spot light
    def __init__(self, parent, 
                                pos=[0.0,0.0,1.0], 
                                color=[1.0,1.0,1.0] ):
        self._parent        = parent
        self._pos           = pos
        self._color         = color
        self.update()


    def update(self, 
                    pos=None, 
                    color=None ):
        if pos is not None:
            self._pos       = pos
        if color is not None:
            self._color     = color
        self._parent.update()