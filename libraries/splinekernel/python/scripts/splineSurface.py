# --- Python imports ---
import numpy as np

# --- Splinekernel imports ---
from pysplinekernel import SurfaceKernel

# --- GLmesh imports ---
from glmesh import convertToSurfaceMesh, MeshApp3D

# --- Lighting imports ---
from lighting import TimedSpotLight

# --- Vispy imports
from vispy import scene

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
geometry        = convertToSurfaceMesh(surf.generatePoints(nSamples=nSamples))

# Generate vertex colors based on their height
colors          = np.transpose( geometry['vertices'], (1,0) )[2]
colors          = colors / (np.max(colors) - np.min(colors))
colors          = np.array( (colors, 1-colors/2.0, 1-colors/2.0) ).transpose((1,0))

# -----------------------------------------------------------
# Create GL window and load mesh
meshApp = MeshApp3D(    geometry,
                        colors=colors,
                        light=TimedSpotLight )
meshApp._view._camera.center           = (0.0,0.0,0.5)
meshApp._mesh._light._posFunctor = lambda t : np.array( (3*np.cos(t), 3*np.sin(t), 2.0), dtype=np.float32 )

meshApp.run()