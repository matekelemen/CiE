from pysplinekernel.surfacekernel import SurfaceKernel

points = [
  [
    0.0, 1.0, 2.0, 0.0, 1.0, 2.0, 0.0, 1.0, 2.0
  ],
  [
    0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0
  ],
  [
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
  ]]


kernel = SurfaceKernel(interpolationPoints=points, meshSize=[3,3])

newPoint = [0.5,1.0]
kernel.addPoint( newPoint )
print(kernel.interpolationPoints)