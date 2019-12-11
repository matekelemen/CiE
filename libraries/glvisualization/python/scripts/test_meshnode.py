# --- Python imports ---
import numpy as np

# --- GLMesh imports ---
from glmesh import MeshNode, MeshVisual, MeshApp3D

# --- GLTexture imports ---
from gltexture import textureFolderPath, loadTexture
from gltexture import emptyTexture, uniformTexture, AggregatedTexture

# --- Lighting imports ---
from lighting import SimpleLight, TimedSpotLight

# --- Vispy imports ---
from vispy.gloo import VertexBuffer
from vispy import scene, app

# -----------------------------------------------------------
'''
torus1  = Torus( 1.0, 0.5, [0.0,0.0,0.0] )
torus2  = Torus( 0.5, 0.2, [1.0, 0.5, 0.2] )

n       = 100
data1   = torus1.evaluate( torus1.u(nSamples=n), torus1.v(nSamples=n), format='list' )
data2   = torus2.evaluate( torus2.u(nSamples=n), torus2.v(nSamples=n), format='list' )

v1      = data1['vertices']
f1      = data1['faces']
v2      = data2['vertices']
f2      = data2['faces']
'''

v1      = np.asarray(   [   [0.0,0.0,0.0],
                            [1.0,0.0,0.0],
                            [1.0,1.0,0.0],
                            [0.0,1.0,0.0]],
                        dtype=np.float32)
f1      = np.asarray(   [   [0,1,2],
                            [0,2,3]],
                            dtype=np.uint32)
tc1     = np.asarray(   [   [0.0,0.0],
                            [1.0,0.0],
                            [1.0,1.0],
                            [0.0,1.0]],
                            dtype=np.float32)
v2      = np.asarray(   [   [0.0,0.0,0.0],
                            [1.0,0.0,0.0],
                            [1.0,0.0,1.0],
                            [0.0,0.0,1.0]],
                        dtype=np.float32)
f2      = np.asarray(   [   [0,3,2],
                            [0,2,1]],
                        dtype=np.uint32)
tc2     = np.asarray(   [   [0.0,0.0],
                            [1.0,0.0],
                            [1.0,1.0],
                            [0.0,1.0]],
                        dtype=np.float32)
v3      = np.asarray(   [   [0.0,0.0,0.0],
                            [0.0,1.0,0.0],
                            [0.0,1.0,1.0],
                            [0.0,0.0,1.0]],
                        dtype=np.float32)
f3      = np.asarray(   [   [0,1,2],
                            [0,2,3]],
                        dtype=np.uint32)
tc3     = np.asarray(   [   [0.0,0.0],
                            [1.0,0.0],
                            [1.0,1.0],
                            [0.0,1.0]],
                        dtype=np.float32)
# -----------------------------------------------------------
textureXY   = loadTexture(textureFolderPath + '/cage.png')
textureXZ   = uniformTexture(size=(512,512), color=(1.0,1.0,1.0))
textureYZ   = uniformTexture( size=(512,512), color=(0.5,0.5,0.5) )
textureDim  = ( textureXY.shape[0]+textureXZ.shape[0]+textureYZ.shape[0], textureXY.shape[1]+textureXZ.shape[1]+textureYZ.shape[1] )

agg         = AggregatedTexture( shape=textureDim )
agg.registerTexture( textureXY, 0, ambientMaterialConstant=0.5, diffuseMaterialConstant=0.5, specularMaterialConstant=1.0 )
agg.registerTexture( textureXZ, 1, ambientMaterialConstant=0.5, diffuseMaterialConstant=0.5, specularMaterialConstant=1.0 )
agg.registerTexture( textureYZ, 2, ambientMaterialConstant=0.5, diffuseMaterialConstant=0.5, specularMaterialConstant=1.0 )

vertexShader, fragmentShader = agg.generateShaderCode()

with open('AggregatedVertexShader.txt','w') as file:
    file.write(vertexShader)

with open('AggregatedFragmentShader.txt','w') as file:
    file.write(fragmentShader)

# -----------------------------------------------------------
root        = MeshNode( vertices=v1, 
                        faces=f1, 
                        textureCoordinates=tc1,
                        objectID=0,
                        materialID=0 )
root.addChild( MeshNode(    vertices=v2,
                            faces=f2,
                            textureCoordinates=tc2,
                            objectID=1,
                            materialID=1) )
root.addChild( MeshNode(    vertices=v3,
                            faces=f3,
                            textureCoordinates=tc3,
                            objectID=2,
                            materialID=2) )

# -----------------------------------------------------------
mApp    = MeshApp3D(    root,
                        agg )
mApp.run()