# --- Python imports ---
import numpy as np

# --- Splinekernel imports ---
from pysplinekernel import SurfaceKernel

# --- Mesh3D imports ---
from mesh3D import TriangleMeshVisual, convertToSurfaceMesh

# --- Lighting imports ---
from lighting import TimedSpotLight

# --- Vispy imports
from vispy.scene.visuals import create_visual_node
from vispy import scene, app

# -----------------------------------------------------
# Spline surface settings
polynomialOrders    = [3,3]
nSamples            = [100,100]

# -----------------------------------------------------
# Define geometry to be interpolated
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

# -----------------------------------------------------
# Spline surface setup
surf = SurfaceKernel(
    interpolationPoints=interpolationPoints,
    polynomialOrders=polynomialOrders )

# Generate spline surface points
surf.samples    = nSamples
geometry        = convertToSurfaceMesh(surf.generatePoints())

# Generate vertex colors as a function of their z coordinate
colors          = np.transpose( geometry['vertices'], (1,0) )[2]
colors          = colors / (np.max(colors) - np.min(colors))
colors          = np.array( (colors, 1-colors/2.0, 1-colors/2.0) ).transpose((1,0))

# -----------------------------------------------------
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
view.camera.center  = (0.0,0.0,0.5)

# Update the VisPy node with the mesh data
mesh = TriangleMesh(    geometry['vertices'],
                        geometry['faces'],
                        colors=colors,
                        light=TimedSpotLight, 
                        camera=view.camera )

# Configure lighting
mesh._light._posFunctor = lambda t : np.array( (3*np.cos(t), 3*np.sin(t), 2.0), dtype=np.float32 )

# Load mesh to the GPU
view.add(mesh)

# Render
plt.show()
app.run()