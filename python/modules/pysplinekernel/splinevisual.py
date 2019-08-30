import numpy as np
from vispy import app, scene
from pysplinekernel.splinekernel import SplineKernel

class SplineVisual(scene.visuals.Line):
    # CONSTRUCTOR AND INITIALZER ----------------------------------------------------------------------
    def __init__(self, *args, **kwargs):
        scene.visuals.Line.__init__(self, *args, **kwargs)
        self.reset()
        
    def reset(self):
        self.unfreeze()
        # Splinekernel
        self.splineKernel = SplineKernel()
        self.splineKernel.push([0.0,0.0])
        self.freeze()

    # MISC FUNCTIONS ------------------------------------------------------------
    def enable(self):
        self._visible = True
        self.draw()

    def disable(self):
        self._visible = False
        self.draw()

    # SPLINE EDIT ---------------------------------------------------------------
    def addInterpolationPoint(self,point):
        self.splineKernel.pop()
        self.splineKernel.push( list(point[0:2]) )
        self.splineKernel.push( [0.0,0.0] )

    def draw(self):
        if len(self.splineKernel.interpolationPoints[0]) > self.splineKernel.polynomialOrder:
            self.splineKernel.getPoints()
            self.set_data( pos=np.transpose(
                np.append( self.splineKernel.curvePoints, np.zeros( [1,len(self.splineKernel.curvePoints[0])] ), axis=0 ),
                [1,0]
                ))
        scene.visuals.Line.draw(self)

    # EVENT HANDLERS ------------------------------------------------------------
    def on_draw(self,event):
        scene.visuals.Line.draw(self)