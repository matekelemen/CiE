
from numpy import float32, load
import numpy as np
from vispy.geometry import MeshData
from vispy.gloo import VertexBuffer
from vispy.io.image import write_png
from vispy.plot import Fig
from vispy.scene.visuals import create_visual_node
from vispy.visuals import Visual
from pysplinekernel.surfacekernel import SurfaceKernel

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
surf.samples    = [100,50]
data            = np.reshape(
                    np.transpose( 
                        np.array(  surf.generatePoints(),dtype=np.float32  ), 
                        axes=(1,2,0)),
                    ( surf.samples[0]*surf.samples[1] ,3))

# Define triangles
elements        = np.zeros( ( 2*(surf.samples[0]-1)*(surf.samples[1]-1), 3 ), dtype=np.uint32 )
k               = 0
for j in range(surf.samples[0]-1):
    for i in range(surf.samples[1]-1):
        jni = j*surf.samples[1]+i
        elements[k] = [ 
            jni, 
            jni+surf.samples[1], 
            jni+1+surf.samples[1]
            ]
        elements[k+1] = [ 
            jni, 
            jni+1, 
            jni+1+surf.samples[1]
            ]
        k+=2

surf = {
    'vert'  : data,
    'faces' : elements
}

vertex_shader = """
varying float colour;
varying float height;

void main() {
    height      = $maxHeight;
    colour      = $position.z;
    gl_Position = $transform(vec4($position, 1));
}
"""

fragment_shader = """
varying float colour;
varying float height;

void main() {
    float p         = max(0.0,colour/height);
    gl_FragColor    = vec4(p,p,p, 1.0);
}
"""
maxHeight   = np.max(np.transpose( data, (1,0) )[2])

class SimpleMeshVisual(Visual):
    def __init__(self, meshdata, color=None, light_vec=(0.0, 0.0, -1.0)):
        Visual.__init__(self, vertex_shader, fragment_shader)

        v = meshdata.get_vertices(indexed='faces').astype(float32)
        self._vertices = VertexBuffer(v)

        v_norm = meshdata.get_vertex_normals(indexed='faces').astype(float32)
        self._normals = VertexBuffer(v_norm)
        self._colors = color
        self._light_vec = light_vec

        self._draw_mode = 'triangle_strip'
        self.set_gl_state('opaque', depth_test=True,  cull_face=True)

    def _prepare_transforms(self, view):
        view.view_program.vert['transform'] = view.get_transform()

    def _prepare_draw(self, view):
        self.shared_program.vert['position']    = self._vertices
        #self.shared_program.vert['normal']      = self._normals
        self.shared_program.vert['maxHeight']   = maxHeight

SimpleMesh = create_visual_node(SimpleMeshVisual)

fig = Fig()
plt = fig[0, 0]
plt._configure_3d()


SKIN_COLOR = 0.94, 0.82, 0.81, 1.
meshdata = MeshData(vertices=surf['vert'], faces=surf['faces'])
mesh = SimpleMesh(meshdata, color=SKIN_COLOR)

plt.view.add(mesh)

plt.view.camera = scene.cameras.ArcballCamera(fov=60)
plt.view.camera.center          = (0.0,0.0,0.5)

fig.show()
'''
canvas = scene.SceneCanvas()
canvas.unfreeze()
canvas.view = canvas.central_widget.add_view()
canvas.view.camera = scene.PanZoomCamera(rect=(-100, -100, 200, 200),
                                               aspect=1.0)
canvas.update()
canvas.show()
'''
app.run()