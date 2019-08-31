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
        #self.splineKernel.push([0.0,0.0])
        self.freeze()

    # MISC FUNCTIONS ------------------------------------------------------------
    def enable(self):
        self._visible = True
        self.draw()

    def disable(self):
        self._visible = False
        self.draw()

    # SPLINE EDIT ---------------------------------------------------------------
    def addPoint(self,point):
        self.splineKernel.push( list(point[0:2]) )
        
    def setPoint(self,point,index=-1):
        if index < len(self.splineKernel.interpolationPoints[0]) and len(self.splineKernel.interpolationPoints[0]) is not 0:
            self.splineKernel.interpolationPoints[0][index] = point[0]
            self.splineKernel.interpolationPoints[1][index] = point[1]

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