from pysplinekernel import *

from vispy.plot import Fig
from vispy.scene.visuals import create_visual_node
import numpy as np

from vispy import scene, app


# Set up spline surface
a = np.sqrt(3/4)
interpolationPoints = np.array([
        [
                [-1.0, -1.0, -1.0, -1.0, -1.0],
                [-0.5, -0.5, -0.5, -0.5, -0.5],
                [ 0.0,  0.0,  0.0,  0.0,  0.0],
                [ 0.5,  0.5,  0.5,  0.5,  0.5],
                [ 1.0,  1.0,  1.0,  1.0,  1.0]
        ],
        [
                [-1.0, -0.5,  0.0,  0.5,  1.0],
                [-1.0, -0.5,  0.0,  0.5,  1.0],
                [-1.0, -0.5,  0.0,  0.5,  1.0],
                [-1.0, -0.5,  0.0,  0.5,  1.0],
                [-1.0, -0.5,  0.0,  0.5,  1.0]
        ],
        [
                [ 0.0,  0.0,  0.0,  0.0,  0.0],
                [ 0.0,  a*a,  a  ,  a*a,  0.0],
                [ 0.0,  a  ,  1.5,  a  ,  0.0],
                [ 0.0,  a*a,  a  ,  a*a,  0.0],
                [ 0.0,  0.0,  0.0,  0.0,  0.0]
        ]
        ])
polynomialOrders    = [3,3]

surf = SurfaceKernel(
    interpolationPoints=interpolationPoints,
    polynomialOrders=polynomialOrders)

# Get points
surf.samples    = [100,100]

geometry = convertToSurfaceMesh(surf.generatePoints())

TriangleMesh = create_visual_node(TriangleMeshVisual)

fig = Fig()
plt = fig[0, 0]
plt._configure_3d()

colors  = np.transpose( geometry['vertices'], (1,0) )[2]
hMax    = np.max(colors)
hMin    = np.min(colors)
colors  = colors/(hMax-hMin)
colors  = np.array( [colors, 0.5*np.ones(len(colors)), 0.5*np.ones(len(colors))] ).transpose((1,0))

mesh = TriangleMesh(geometry['vertices'],geometry['faces'],colors=colors,light=TimedSpotLight)
mesh._light._lightPos = np.array([0.0,0.0,5.0],dtype=np.float32)

plt.view.add(mesh)

plt.view.camera = scene.cameras.ArcballCamera(fov=0)
plt.view.camera.center          = (0.0,0.0,0.5)

fig.show()
app.run()