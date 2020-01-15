# --- Python Imports ---
import numpy as np
from timeit import default_timer

# --- Internal Imports ---
from pydropmerge import *

# --- CSG Imports ---
from pycsg import OctreeCanvas

# --- Vispy Imports ---
from vispy import app

# -----------------------------------------------------------
nDim                = 2
treeDepth           = 9
boundary            = False
visual              = "cell"

tree    = DynamicTree( [0.0 for i in range(nDim)], 4.0 )


def on_timer(canvas):

    #t0 = default_timer()
    tree.offset(canvas.t)
    #print("Offsetting:\t" + str(default_timer()-t0))

    #t0 = default_timer()
    tree.divide(treeDepth)
    #print("Dividing:\t" + str(default_timer()-t0))

    #t0 = default_timer()
    data    = collectNodes(tree)
    #print("Collecting:\t" + str(default_timer()-t0))

    #t0 = default_timer()
    data    = { "length"    : data.edgeLengths(),
                "center"    : data.centers(),
                "boundary"  : data.boundaries()}
    #print("Organizing:\t" + str(default_timer()-t0))

    #t0 = default_timer()
    canvas.updateData(data)
    #print("Rendering:\t" + str(default_timer()-t0) + "\n")


tree.offset(0.0)
tree.divide(treeDepth)
data    = collectNodes(tree)
data    = { "length"    : data.edgeLengths(),
            "center"    : data.centers(),
            "boundary"  : data.boundaries()}
canvas  = OctreeCanvas(data,boundary=boundary,visual=visual,animation=on_timer)
del data


canvas.view.camera.rect = (-2.0,-2.0,4.0,4.0)
canvas.show()
app.run()