from pysplinekernel import *

surf = SurfaceKernel()

surf.addPoint([1.0,1.0,1.0])
surf.addPoint([2.0,2.0,2.0])
surf.addPoint([3.0,3.0,3.0])

print(surf.interpolationPoints)