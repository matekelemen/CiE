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
minimumDisc = csg.MinimumEnclosingDisc(points)
print('Number of restarts: ' + str(minimumDisc.build(1e-10)))
center      = minimumDisc.getCenter()
radius      = minimumDisc.getRadius()

print('Disc center: ' + str(center))
print('Disc radius: ' + str(radius))

# Convert points to match matplotlib input format, then plot them
points = np.transpose( np.asarray(points), axes=(1,0) )
plt.plot( points[0,:], points[1,:], 'o' )

# Add the disc
ax = plt.gca()
ax.add_artist( plt.Circle(center,radius,color='r',alpha=0.5) )

ax.set_xlim( (center[0]-radius,center[0]+radius) )
ax.set_ylim( (center[1]-radius,center[1]+radius) )
ax.axis('equal')

# Render plot
plt.show()