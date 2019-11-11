# --- Python imports ---
import numpy as np

# --- Visualization imports ---
from glmesh import ParametricSurface, convertToSurfaceMesh, MeshApp3D

# --- Lighting imports ---
from lighting import TimedSpotLight

# -----------------------------------------------------------
# Torus parameters
radius1     = 1.0
radius2     = 0.2

U           = np.linspace(0.0,2*np.pi,100)
V           = np.linspace(0.0,-2*np.pi,100)

torus   = ParametricSurface(    (   lambda u,v : np.cos(u)*(radius1+radius2*np.cos(v)),
                                    lambda u,v : np.sin(u)*(radius1+radius2*np.cos(v)),
                                    lambda u,v : radius2*np.sin(v) ),
                                domain=((min(U),max(U)),(min(V),max(V))) )
# -----------------------------------------------------------
# Create GL window
meshApp     = MeshApp3D(    convertToSurfaceMesh( torus.evaluate(U,V) ),
                            light=TimedSpotLight,
                            colors=(1.0,1.0,1.0) )

meshApp._mesh._light._colorFunctor  = lambda t : (1.0,1.0,1.0)
meshApp._mesh._light._posFunctor    = lambda t : (0.0,0.0,10*(radius1+radius2)*np.cos(t))

meshApp.run()