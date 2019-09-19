import numpy as np
from vispy.geometry import MeshData
from vispy.gloo import VertexBuffer
from vispy.io.image import write_png
from vispy.plot import Fig
from vispy.scene.visuals import create_visual_node
from vispy.visuals import Visual

from pysplinekernel.surfacekernel import SurfaceKernel
from pysplinekernel.light import SimpleLight, TimedSpotLight

from vispy import scene, app


from timeit import default_timer

# -----------------------------------------------------
# DEFAULT SHADERS
# -----------------------------------------------------
defaultVertexShader = """
varying vec3 position;
varying vec3 normal;
varying vec4 color;

void main() {
    normal      = $normal;
    color       = $color;
    position    = $position;
    gl_Position = $transform(vec4(position,1));
}
"""

defaultFragmentShader = """
varying vec3 position;
varying vec3 normal;
varying vec4 color;

void main() {
    vec3 lightPos       = position - vec3($lightPos);
    float intensity     = dot( normal, normalize(lightPos) );
    intensity           = min( max(intensity,0.0), 1.0 );
    gl_FragColor        = vec4( intensity*vec3($lightColor),1.0 )*color;
}
"""

# -----------------------------------------------------
# MESH
# -----------------------------------------------------
class TriangleMeshVisual(Visual):
    def __init__(self,vertices,faces,colors=None,light=None,vertexShader=defaultVertexShader,fragmentShader=defaultFragmentShader):
        Visual.__init__(self,vertexShader,fragmentShader)
        
        if colors is None:
            colors  = 0.5* np.ones(np.shape(vertices),dtype=np.float32)

        if light is None:
            light = SimpleLight(self)
        else:
            light = light(self)

        self._meshData  = MeshData(vertices=vertices,faces=faces, vertex_colors=colors)
        self._vertices          = None
        self._colors            = None
        self._normals           = None
        self._light             = light
        self._draw_mode = 'triangle_strip'
        self.set_gl_state('opaque',depth_test=True,cull_face=True)

        self.updateMesh(vertices=vertices,faces=faces,colors=colors)


    def _prepare_transforms(self, view):
        view.view_program.vert['transform'] = view.get_transform()


    def _prepare_draw(self,view):
        self.shared_program.vert['position']    = self._vertices
        self.shared_program.vert['normal']      = self._normals
        self.shared_program.vert['color']       = self._colors
        self.shared_program.frag['lightPos']    = self._light._lightPos
        self.shared_program.frag['lightColor']  = self._light._lightColor


    def updateMesh(self,vertices=None,faces=None,colors=None):
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





def convertToSurfaceMesh(surfacePoints):
    size0 = len(surfacePoints[0])
    size1 = len(surfacePoints[0][0])
    geometry = {'vertices':[], 'faces':[]}
    geometry['vertices'] = np.reshape(
                    np.transpose( 
                        np.array( surfacePoints ,dtype=np.float32  ), 
                        axes=(1,2,0)),
                    ( size0*size1 ,3))
    geometry['faces'] = np.zeros( ( 2*(size0-1)*(size1-1), 3 ), dtype=np.uint32 )
    k = 0
    for j in range(size0-1):
        for i in range(size1-1):
            jni = j*size1+i
            geometry['faces'][k] = [ 
                jni, 
                jni+size1+1, 
                jni+size1
                ]
            geometry['faces'][k+1] = [ 
                jni, 
                jni+1, 
                jni+1+size1
                ]
            k+=2
    return geometry