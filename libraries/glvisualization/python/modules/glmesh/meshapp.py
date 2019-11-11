# --- Python imports ---
import numpy as np

# --- Internal imports ---
from glmesh import TriangleMeshVisual
from glmesh import defaultVertexShader, defaultFragmentShader

# --- Lighting imports ---
from lighting import SimpleLight

# --- Vispy imports ---
from vispy import scene, app

# -----------------------------------------------------
class MeshApp3D:
    def __init__(self,
                        surfaceMesh,
                        camera=None,
                        bgcolor=(0.2,0.2,0.2),
                        border_color=(0.0,0.0,0.0),
                        border_width=1,
                        **kwargs ):
        # Vispy init
        self._meshNode          = scene.visuals.create_visual_node(TriangleMeshVisual)
        self._canvas            = scene.SceneCanvas(    keys='interactive',
                                                        size=(1024,768) )
        self._view              = self._canvas.central_widget.add_view(     bgcolor=bgcolor,
                                                                            border_color=border_color,
                                                                            border_width=border_width )

        # Default arguments
        if camera is None:
            self._view.camera   = scene.cameras.ArcballCamera(  fov=0, 
                                                                center=(0.0,0.0,0.0), 
                                                                parent=self._view )
        else:
            self._view.camera   = camera( parent=self._view )

        # Create visual
        self._mesh              = self._meshNode(   surfaceMesh['vertices'],
                                                    surfaceMesh['faces'],
                                                    camera=self._view.camera,
                                                    **kwargs )
        
        # Adjust lighting
        vertices                    = np.transpose(  self._mesh._meshData.get_vertices() ,(1,0) )
        self._mesh._light._pos      = ( ( max(vertices[0]) - min(vertices[0]) )/2.0,
                                        ( max(vertices[1]) - min(vertices[1]) )/2.0,
                                          max(vertices[2]) + (max(vertices[2])-min(vertices[2]))*10     )
        self._mesh._light._color    = np.array( (1.0,1.0,1.0), dtype=np.float32 )

        self._view.add(self._mesh)
        


    def run(self):
        self._canvas.show()
        app.run()