# -----------------------------------------------------------
# --- Splinekernel imports ---
from pysplinekernel import SurfaceKernel

# --- GL Visualization imports ---
from lighting import SimpleLight
from mesh3D import TriangleMeshVisual, convertToSurfaceMesh

# --- VisPy imports ---
from vispy.plot import Fig
from vispy.scene.visuals import create_visual_node
from vispy import scene, app

# --- Python imports ---
import numpy as np
# -----------------------------------------------------------

# Settings - sphere
radius                  = 1.0
offset                  = [ 0.0, 0.0, 0.0 ]
# Settings - spline
nInterpolationPoints    = [15,15]
polynomialOrders        = [3,3]
nSamples                = [80,80]

# -----------------------------------------------------------
# Define sphere
class Sphere:
        def __init__(self,radius=1.0,offset=[0.0,0.0,0.0]):
                self._radius    = radius
                if len(offset) is 2:
                        offset.append(0.0)
                if len(offset) is 3:
                        self._offset    = offset
                else:
                        raise ValueError('Invalid offset!')


        def surfacePoint(self, u, v):
                return [
                        self._radius*np.cos(v)*np.cos(u) - self._offset[0],
                        self._radius*np.cos(v)*np.sin(u) - self._offset[1],
                        self._radius*np.sin(v) - self._offset[2]
                ]


# Get points on the sphere (interpolation points)
S                       = Sphere(offset=[0.0,0.0,0.0])
interpolationPoints     = [[],[],[]]
for i,v in enumerate( np.linspace(np.pi/2,-np.pi/2,num=nInterpolationPoints[0]+1) ):
        interpolationPoints[0].append([])
        interpolationPoints[1].append([])
        interpolationPoints[2].append([])
        for j,u in enumerate( np.linspace(0.0,2*np.pi,num=nInterpolationPoints[1]+1) ):
                point   = S.surfacePoint(u,v)
                interpolationPoints[0][i].append(point[0])
                interpolationPoints[1][i].append(point[1])
                interpolationPoints[2][i].append(point[2])

# -----------------------------------------------------------
# Create spline surface
surf = SurfaceKernel(
    interpolationPoints=np.array(interpolationPoints),
    polynomialOrders=polynomialOrders)

# Sample the spline surface and convert it to a triangle mesh
surf.samples    = nSamples
geometry        = convertToSurfaceMesh(surf.generatePoints())

# Define vertex colors
colors  = ( 0.5, 0.5, 0.8 )
colors  = [ colors for i in range( len(geometry['vertices']) ) ]
colors  = np.array( colors, dtype=np.float32 )

# -----------------------------------------------------------
# Add mesh as VisPy node
TriangleMesh = create_visual_node(TriangleMeshVisual)

# Create a new OpenGL window
plt = scene.SceneCanvas(        keys='interactive', 
                                size=(1024, 768) )
                                
view = plt.central_widget.add_view(     bgcolor=(0.2,0.2,0.2),
                                        border_color=(0.0,0.0,0.0),
                                        border_width=1 )

# Add and configure camera
view.camera = scene.cameras.ArcballCamera(fov=0)
view.camera.center  = (0.0,0.0,0.0)

# Update the VisPy node with the mesh data
mesh = TriangleMesh(    geometry['vertices'],
                        geometry['faces'],
                        colors=colors,
                        light=SimpleLight, 
                        camera=view.camera )

# Configure lighting
mesh._light._pos   = np.array( [0.0,0.0,10.0], dtype=np.float32 )
mesh._light._color = np.array( [1.0,1.0,1.0], dtype=np.float32 )

# Load mesh to the GPU
view.add(mesh)

# Render
plt.show()
app.run()