# --- Python imports ---
import numpy as np

# --- Vispy imports ---
from vispy import scene, app

# --- Visualization imports ---
from glmesh import defaultFragmentShaderWithTexture, defaultVertexShaderWithTexture
from glmesh import MeshApp3D, convertToSurfaceMesh, ParametricSurface
from gltexture import loadTexture, textureFolderPath
from lighting import TimedSpotLight

# -----------------------------------------------------------
# Texture
texture             = loadTexture( textureFolderPath + '/cage.png' )

# Geometry
geom                = ParametricSurface(    (   lambda u,v : np.cos(u)*(1+0.5*np.cos(v)),
                                                lambda u,v : np.sin(u)*(1+0.5*np.cos(v)),
                                                lambda u,v : 0.5*np.sin(v) ),
                                            domain=((0.0,2*np.pi),(0.0,-2*np.pi)) )

nSamples            = 100
vertices            = geom.evaluate(    np.linspace(geom._domain[0][0],geom._domain[0][1],num=nSamples),
                                        np.linspace(geom._domain[1][0],geom._domain[1][1],num=nSamples) )
geometry            = convertToSurfaceMesh( vertices )

# Texture - geometry map
textureCoordinates  = np.stack( np.meshgrid(    np.linspace(0.0,1.0,num=nSamples),
                                                np.linspace(0.0,1.0,num=nSamples) ),
                                axis=2 )
textureCoordinates  = np.reshape( textureCoordinates, (textureCoordinates.shape[0]*textureCoordinates.shape[1],2) )

# Create OpenGL window
meshApp             = MeshApp3D(    geometry,
                                    texture=texture,
                                    textureCoordinates=textureCoordinates,
                                    vertexShader=defaultVertexShaderWithTexture,
                                    fragmentShader=defaultFragmentShaderWithTexture,
                                    light=TimedSpotLight )

meshApp._mesh._light._colorFunctor  = lambda t : (1.0,1.0,1.0)
meshApp._mesh._light._posFunctor    = lambda t : (0.0,0.0,10*np.cos(t))
meshApp._mesh._light._color  = (1.0,1.0,1.0)

meshApp.run()