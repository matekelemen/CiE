# --- Python imports ---
import numpy as np

# --- GLMesh imports ---
from glmesh import MeshNode, MeshVisual, Torus

# --- Lighting imports ---
from lighting import SimpleLight

# --- Vispy imports ---
from vispy.gloo import VertexBuffer
from vispy import scene, app

# -----------------------------------------------------------
torus1  = Torus( 1.0, 0.5, [0.0,0.0,0.0] )
torus2  = Torus( 0.5, 0.2, [1.0, 0.5, 0.2] )

n       = 100
data1   = torus1.evaluate( torus1.u(nSamples=n), torus1.v(nSamples=n), format='list' )
data2   = torus2.evaluate( torus2.u(nSamples=n), torus2.v(nSamples=n), format='list' )

v1          = data1['vertices']

f1          = data1['faces']

v2          = data2['vertices']

f2          = data2['faces']

# -----------------------------------------------------------
root        = MeshNode( vertices=v1, faces=f1 )
root.addChild( MeshNode(    vertices=v2,
                            faces=f2,
                            objectID=1,
                            materialID=2) )

# -----------------------------------------------------------
MeshVisualNode  = scene.visuals.create_visual_node(MeshVisual)

canvas          = scene.SceneCanvas(    keys='interactive',
                                        size=(1024,768)     )

view            = canvas.central_widget.add_view( bgcolor=(0.2,0.2,0.2) )
view.camera     = scene.cameras.ArcballCamera(  fov=0,
                                                center=(0.0,0.0,0.0),
                                                parent=view )

meshVisual      = MeshVisualNode(   root,
                                    light=SimpleLight,
                                    camera=view.camera )

view.add(meshVisual)
canvas.show()
app.run()