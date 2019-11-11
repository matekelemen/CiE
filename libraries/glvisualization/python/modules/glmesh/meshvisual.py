# --- Python imports ---
import numpy as np

# --- Vispy imports ---
from vispy.geometry import MeshData
from vispy.gloo import VertexBuffer, Texture2D, wrappers
from vispy.visuals import Visual

# --- Lighting imports ---
from lighting import SimpleLight

# --- Internal imports ---
from glmesh import defaultVertexShader, defaultFragmentShader

# -----------------------------------------------------
# MESH
# -----------------------------------------------------
class TriangleMeshVisual(Visual):
    def __init__(   self, vertices, faces, 
                        colors=None,
                        texture=None,
                        textureCoordinates=None,
                        light=None,
                        ambientMaterialConstant=0.5,
                        diffuseMaterialConstant=0.4,
                        specularMaterialConstant=0.3,
                        vertexShader=defaultVertexShader,
                        fragmentShader=defaultFragmentShader,
                        camera=None ):
        Visual.__init__(self,vertexShader,fragmentShader)

        wrappers.set_cull_face(mode='back')
        
        # Default arguments
        if colors is None and texture is None:
            colors  = 0.5* np.ones(np.shape(vertices),dtype=np.float32)

        if light is None:
            light = SimpleLight(self, color=(1.0,1.0,1.0))
        else:
            light = light(self)

        # Geometry setup
        self._meshData                  = MeshData(vertices=vertices,faces=faces, vertex_colors=colors)
        self._vertices                  = None
        self._colors                    = None
        self._texture                   = None
        self._textureCoordinates        = None
        self._normals                   = None
        self._draw_mode                 = 'triangles'
        
        self.set_gl_state(  'opaque',
                            depth_test=True,
                            cull_face=True )

        # Light setup
        self._light                     = light
        self._camera                    = camera

        self._ambientMaterialConstant   = ambientMaterialConstant
        self._diffuseMaterialConstant   = diffuseMaterialConstant
        self._specularMaterialConstant  = specularMaterialConstant

        # Initial updates
        self.updateMesh(    vertices=vertices,
                            faces=faces,
                            colors=colors,
                            texture=texture,
                            textureCoordinates=textureCoordinates )
        self.updateLight()


    # --- Function override ---
    def _prepare_transforms(self, view):
        view.view_program.vert['transform']         = view.get_transform()


    def _prepare_draw(self,view):
        self.updateMesh()
        self.updateLight()


    # --- State update ---
    def updateMesh(self,    vertices=None,
                            faces=None,
                            colors=None,
                            texture=None,
                            textureCoordinates=None ):
        # Set
        if vertices is not None:
            self._meshData.set_vertices(verts=vertices)
        if faces is not None:
            self._meshData.set_faces(faces)
        if colors is not None:
            self._meshData.set_vertex_colors(colors)

        # Update
        if vertices is not None or faces is not None:
            self._vertices = VertexBuffer(self._meshData.get_vertices(indexed='faces').astype(np.float32))
        if faces is not None and vertices is not None:
            self._normals = VertexBuffer(
                self._meshData.get_vertex_normals(indexed='faces').astype(np.float32))
        if colors is not None:
            self._colors = VertexBuffer(
                self._meshData.get_vertex_colors(indexed='faces').astype(np.float32))
        if texture is not None:
            self._texture = Texture2D(  np.array(texture,dtype=np.float32),
                                        interpolation='linear',
                                        wrapping='mirrored_repeat' )
        if textureCoordinates is not None:
            textureMesh     = MeshData(     vertices=textureCoordinates,
                                            faces=self._meshData.get_faces())
            self._textureCoordinates = VertexBuffer( textureMesh.get_vertices(indexed='faces').astype(np.float32) )

        # Update GPU - geometry
        self.shared_program.vert['position']                = self._vertices
        self.shared_program.vert['normal']                  = self._normals

        # Update GPU - color
        if self._texture is None:
            self.shared_program.vert['color']               = self._colors
        else:
            self.shared_program.vert['textureCoordinates']  = self._textureCoordinates
            self.shared_program['u_objectTexture']          = self._texture


    def updateLight(self):
        self.shared_program.frag['ambientLight']                = self._light._ambient
        self.shared_program.frag['ambientMaterialConstant']     = self._ambientMaterialConstant
        self.shared_program.frag['diffuseMaterialConstant']     = self._diffuseMaterialConstant
        self.shared_program.frag['specularMaterialConstant']    = self._specularMaterialConstant
        self.shared_program.frag['lightPos']                    = self._light._pos
        self.shared_program.frag['lightColor']                  = self._light._color
        self.shared_program.frag['cameraPos']                   = np.array( self._camera._quaternion.rotate_point( [0.0,1.0,0.0] ), dtype=np.float32)
        #print(self._camera.distance)





def convertToSurfaceMesh(grid):
    size0 = len(grid[0])
    size1 = len(grid[0][0])
    geometry = {'vertices':[], 'faces':[]}
    geometry['vertices'] = np.reshape(
                    np.transpose( 
                        np.array( grid ,dtype=np.float32  ), 
                        axes=(1,2,0)),
                    ( size0*size1 ,3))
    geometry['faces'] = np.zeros( ( 2*(size0-1)*(size1-1), 3 ), dtype=np.uint32 )
    k = 0
    for j in range(size0-1):
        for i in range(size1-1):
            jni = j*size1+i
            geometry['faces'][k] = [ 
                jni, 
                jni+size1,
                jni+size1+1
                ]
            geometry['faces'][k+1] = [ 
                jni, 
                jni+1+size1,
                jni+1
                ]
            k+=2
    return geometry