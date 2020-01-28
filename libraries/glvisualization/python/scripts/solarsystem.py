# --- Python imports ---
import numpy as np

# --- GLMesh imports ---
from glmesh import Sphere
from glmesh import convertToSurfaceMesh
from glmesh import MeshNode, MeshVisual, MeshApp3D

# --- GLTexture imports ---
from gltexture import textureFolderPath, loadTexture
from gltexture import uniformTexture, AggregatedTexture

# -----------------------------------------------------------
# Define geometries
sphere1     = Sphere(1.0,[0.0,0.0,0.0])
sphere2     = Sphere(1.0,[3.0,0.0,0.0])

# Sample
nSamples    = 10
sphere1     = convertToSurfaceMesh( sphere1.evaluate(sphere1.u(nSamples=nSamples), sphere1.v(nSamples=nSamples)) )
sphere2     = convertToSurfaceMesh( sphere2.evaluate(sphere2.u(nSamples=nSamples), sphere2.v(nSamples=nSamples)) )

# Load geometries in mesh nodes
root        = MeshNode( parent=None,
                        vertices=sphere1["vertices"],
                        faces=sphere1["faces"],
                        objectID=0,
                        materialID=0    )

root.addChild( MeshNode(    parent=root,
                            vertices=sphere2["vertices"],
                            faces=sphere2["faces"],
                            objectID=1,
                            materialID=1) )

# Create app
meshApp = MeshApp3D(root)

meshApp.run()