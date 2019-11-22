# --- CSG imports ---
import pycsg as csg

# --- Python imports ---
import numpy as np
import matplotlib.pyplot as plt
import random

# -------------------------------------------------------
# Generate points
nSamples    = 25

xRange      = (-random.randrange(0.0,100.0), random.randrange(0.0,100.0))
yRange      = (-random.randrange(0.0,100.0), random.randrange(0.0,100.0))
points      = np.empty((nSamples,2),dtype=np.float64)
for i in range(nSamples):
    points[i,:] = [random.randrange(*xRange),random.randrange(*yRange)]

# Build disc
minimumDisc     = csg.MinimumEnclosingDisc(points)
numRestarts     = minimumDisc.build(1e-10)
print('Number of restarts: ' + str(numRestarts))

    
center          = minimumDisc.getCenter()
radius          = minimumDisc.getRadius()
activeIndices   = minimumDisc.getActiveIndices()

print('Disc center: ' + str(center))
print('Disc radius: ' + str(radius))

# Convert points to match matplotlib input format, then plot them
points = np.transpose( np.asarray(points), axes=(1,0) )
plt.plot( points[0,:], points[1,:], '+', color=(0,0,1) )

plt.plot( points[0,activeIndices], points[1,activeIndices], '+', color=(1,0,0) )

# Add the disc
ax = plt.gca()
ax.add_artist( plt.Circle(  center,
                            radius,
                            color=(0.5,0.5,0.5),
                            edgecolor=(0.8,0.8,0.8),
                            linewidth=1.0,
                            alpha=0.5) )

ax.set_xlim( (center[0]-radius,center[0]+radius) )
ax.set_ylim( (center[1]-radius,center[1]+radius) )
ax.axis('equal')

# Render plot
plt.show()