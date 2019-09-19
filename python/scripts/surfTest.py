from pysplinekernel.surfacekernel import SurfaceKernel
from pysplinekernel.meshvisual import TriangleMeshVisual, convertToSurfaceMesh
from pysplinekernel.light import SimpleLight, TimedSpotLight

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
surf.samples    = [50,50]

geometry = convertToSurfaceMesh(surf.generatePoints())

TriangleMesh = create_visual_node(TriangleMeshVisual)

fig = Fig()
plt = fig[0, 0]
plt._configure_3d()

mesh = TriangleMesh(geometry['vertices'],geometry['faces'],light=SimpleLight)
mesh._light._lightPos = np.array([0.0,0.0,5.0],dtype=np.float32)

plt.view.add(mesh)

plt.view.camera = scene.cameras.ArcballCamera(fov=60)
plt.view.camera.center          = (0.0,0.0,0.5)

fig.show()
app.run()