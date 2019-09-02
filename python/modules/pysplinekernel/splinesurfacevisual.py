from vispy import scene
from pysplinekernel.surfacekernel import SurfaceKernel

class SplineSurfaceVisual(scene.visuals.SurfaceVisual):
    def __init__(self, *args, **kwargs):
        scene.visuals.SurfaceVisual.__init__(*args,**kwargs)
        self.reset()

    def reset(self):
        self.unfreeze()
        self.surfaceKernel = SurfaceKernel()
        self.freeze()

    # SET/GET ---------------------------------------------------------
    

    