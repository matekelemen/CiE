# --- Python imports ---
import numpy as np

# --- Internal imports ---
from glmesh import MeshNode

# --- Vispy imports ---
from vispy.gloo import VertexBuffer

# -----------------------------------------------------------
vertices    = np.asarray(   [   [0.0,0.0,0.0],
                                [1.0,0.0,0.0],
                                [1.0,1.0,0.0],
                                [0.0,1.0,0.0]],
                            dtype=np.float32)

faces       = np.asarray(   [   [0,1,2],
                                [0,2,3] ],
                            dtype=np.uint32)

# -----------------------------------------------------------
root        = MeshNode( vertices=vertices, faces=faces )
root.addChild( MeshNode(    vertices=vertices,
                            faces=faces,
                            objectID=1,
                            materialID=2) )

# -----------------------------------------------------------
mesh        = root.getCompiledMesh()