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
r1                  = 1.0
r2                  = 0.5
geom                = ParametricSurface(    (   lambda u,v : np.cos(u)*(r1+r2*np.cos(v)),
                                                lambda u,v : np.sin(u)*(r1+r2*np.cos(v)),
                                                lambda u,v : r2*np.sin(v) ),
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
                                    light=TimedSpotLight,
                                    specularMaterialConstant=0.75 )

meshApp._mesh._light._colorFunctor  = lambda t : (1.0,1.0,1.0)
meshApp._mesh._light._posFunctor    = lambda t : (0.0,0.0,10*np.cos(t))
meshApp._mesh._light._color  = (1.0,1.0,1.0)

meshApp.run()