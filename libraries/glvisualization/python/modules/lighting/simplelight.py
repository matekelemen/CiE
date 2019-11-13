import numpy as np

class SimpleLight:
    # Simple spot light
    def __init__(self,  parent=None, 
                            pos=[0.0,0.0,1.0], 
                            color=[1.0,1.0,1.0],
                            ambient=[0.2,0.2,0.2] ):

        self._parent        = parent
        self._pos           = pos
        self._color         = color
        self._ambient       = ambient
        #self.update()


    def update(self, 
                    pos=None, 
                    color=None,
                    ambient=None ):
        if pos is not None:
            self._pos       = pos
        if color is not None:
            self._color     = color
        if ambient is not None:
            self._ambient   = ambient
        self._parent.updateLight()


    def setParent(self,parent):
        self._parent = parent