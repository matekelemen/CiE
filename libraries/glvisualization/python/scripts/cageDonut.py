import numpy as np
from PIL import Image
import os
from vispy import scene, app
from glmesh import TriangleMeshVisual, defaultFragmentShaderWithTexture, defaultVertexShaderWithTexture, convertToSurfaceMesh, ParametricSurface
from lighting import TimedSpotLight

pth = os.path.realpath(__file__)[:-13]
# ----------------------------

# Texture
texture             = np.asarray( Image.open(pth + '/../data/glvisualization/textures/cage.png'), dtype=np.float32 )[:,:,:3] / 255.0

# Geometry
'''
geom                = ParametricSurface(    (   lambda u,v : np.cos(2*np.pi*v)*np.cos(2*np.pi*u),
                                                lambda u,v : np.cos(2*np.pi*v)*np.sin(2*np.pi*u),
                                                lambda u,v : np.sin(2*np.pi*v) ) )
'''
'''
geom                = ParametricSurface(    (   lambda u,v : u,
                                                lambda u,v : v,
                                                lambda u,v : 0.0 ) )
'''

geom    = ParametricSurface(    (   lambda u,v : np.cos(u)*(1+0.5*np.cos(v)),
                                    lambda u,v : np.sin(u)*(1+0.5*np.cos(v)),
                                    lambda u,v : 0.5*np.sin(v) ),
                                domain=((0.0,2*np.pi),(0.0,-2*np.pi)) )

nSamples            = 100
vertices            = geom.evaluate(    np.linspace(geom._domain[0][0],geom._domain[0][1],num=nSamples),
                                        np.linspace(geom._domain[1][0],geom._domain[1][1],num=nSamples) )
geometry            = convertToSurfaceMesh( vertices )

'''
X, Y                = np.meshgrid(  np.linspace(0.0,1.0,num=texture.shape[1]),
                                    np.linspace(0.0,1.0,num=texture.shape[0]) )
vertices            = np.stack( (X,Y,np.zeros( X.shape,dtype=np.float32 )), axis=2 )
geometry            = convertToSurfaceMesh( np.transpose(vertices,(2,0,1)) )
textureCoordinates  = np.stack( (X,Y), axis=2 )
'''

# Texture - geometry map
textureCoordinates  = np.stack( np.meshgrid(    np.linspace(0.0,1.0,num=nSamples),
                                                np.linspace(0.0,1.0,num=nSamples) ),
                                axis=2 )
textureCoordinates  = np.reshape( textureCoordinates, (textureCoordinates.shape[0]*textureCoordinates.shape[1],2) )

# Add mesh as VisPy node
TriangleMesh = scene.visuals.create_visual_node(TriangleMeshVisual)


# Create a new OpenGL window
plt = scene.SceneCanvas(        keys='interactive', 
                                size=(1024, 768) )

view = plt.central_widget.add_view(     bgcolor=(0.2,0.2,0.2),
                                        border_color=(0.0,0.0,0.0),
                                        border_width=1 )

# Add and configure camera
geomCenter  = np.array([    ( np.max(geometry['vertices'][:,0]) + np.min(geometry['vertices'][:,0]) )/2.0,
                            ( np.max(geometry['vertices'][:,1]) + np.min(geometry['vertices'][:,1]) )/2.0,
                            ( np.max(geometry['vertices'][:,2]) + np.min(geometry['vertices'][:,2]) )/2.0  ],
                        dtype=np.float32)
view.camera = scene.cameras.ArcballCamera(fov=0,distance=2.0)
view.camera.center  = geomCenter

mesh    = TriangleMesh(             geometry['vertices'],
                                    geometry['faces'],
                                    texture=texture,
                                    textureCoordinates=textureCoordinates,
                                    vertexShader=defaultVertexShaderWithTexture,
                                    fragmentShader=defaultFragmentShaderWithTexture,
                                    camera=view.camera,
                                    light=TimedSpotLight )

mesh._light._colorFunctor  = lambda t : (1.0,1.0,1.0)
mesh._light._posFunctor    = lambda t : (0.0,0.0,10*np.cos(t))

mesh._light._pos = np.array( geomCenter + [0.0,0.0,0.0], dtype=np.float32)
#mesh._light._pos    = [0.0,0.0,-10.0]
mesh._light._color  = (1.0,1.0,1.0)

# Load mesh to the GPU
view.add(mesh)

# Render
plt.show()
app.run()
