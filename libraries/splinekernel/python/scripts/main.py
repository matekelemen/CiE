from pysplinekernel import *
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

'''
grid = np.array([
        [
                [ 0.0, -0.3, -0.4, -0.2],
                [ 1.0,  0.7,  0.5,  0.7],
                [ 2.0,  1.6,  1.4,  1.6],
                [ 3.0,  2.5,  2.3,  2.4]
        ],
        [
                [ 0. ,  1. ,  2. ,  3.1],
                [-0.3,  0.7,  1.6,  2.7],
                [-0.2,  0.8,  1.7,  2.8],
                [ 0.1,  1.1,  2.2,  3.2]
        ],
        [
                [ 1.2,  0.9,  0.9,  1.3],
                [ 0.6,  0.7,  0.8,  0.9],
                [ 1.3,  0.7,  0.6,  1.4],
                [ 0.5,  0.5,  0.7,  0.5]
        ]
        ])
'''
a = np.sqrt(3/4)
grid = np.array([
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
                [ 0.0,  a  ,  1.0,  a  ,  0.0],
                [ 0.0,  a*a,  a  ,  a*a,  0.0],
                [ 0.0,  0.0,  0.0,  0.0,  0.0]
        ]
        ])

print(grid[0])
surf = pysplinekernel.SurfaceKernel(interpolationPoints=grid)
surf.polynomialOrders   = [4,4]
surf.samples            = [15,15]
surfacePoints           = surf.generatePoints()

fig = plt.figure( )
ax = fig.gca( projection='3d' )

fig.tight_layout()

ax.plot_surface( 
        surfacePoints[0], 
        surfacePoints[1], 
        surfacePoints[2],
        alpha=0.5
        )
ax.plot_wireframe( grid[0], grid[1], grid[2], color="black", alpha=0.75 )
ax.scatter( grid[0], grid[1], grid[2], color='red' )
plt.show()

# Instantiate SplineBuilder
#builder = BSplineSurface.SurfaceGUI()
    
# Define events
#motionEvent = builder.ax.figure.canvas.mpl_connect('motion_notify_event', builder.onMotion)
#clickEvent = builder.ax.figure.canvas.mpl_connect('button_press_event', builder.onClick)
#keyPressEvent = builder.ax.figure.canvas.mpl_connect('key_press_event', builder.onKeyPress)

#plt.show()
