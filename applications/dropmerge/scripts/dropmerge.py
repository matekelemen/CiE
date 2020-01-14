# --- Python Imports ---
import numpy as np

# --- Internal Imports ---
from pydropmerge import *

# --- CSG Imports ---
from pycsg import OctreeCanvas

# --- Vispy Imports ---
from vispy import app

# -----------------------------------------------------------
treeDepth           = 5

tree    = DynamicTree( [0.0,0.0,0.0], 4.0 )


def on_timer(canvas):
    tree.offset(canvas.t)
    tree.divide(treeDepth)

    data    = collectNodes(tree)

    tree.divide(treeDepth)
    data    = collectNodes(tree)
    data    = { "length"    : data.edgeLengths(),
                "center"    : data.centers(),
                "boundary"  : data.boundaries()}

    canvas.updateData(data)


tree.offset(0.0)
tree.divide(treeDepth)
data    = collectNodes(tree)
data    = { "length"    : data.edgeLengths(),
            "center"    : data.centers(),
            "boundary"  : data.boundaries()}
canvas  = OctreeCanvas(data,boundary=True,visual="point",animation=on_timer)
del data


canvas.view.camera.rect = (-2.0,-2.0,4.0,4.0)
canvas.show()
app.run()