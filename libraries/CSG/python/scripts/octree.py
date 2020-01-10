import PyQt5

# --- Internal Imports ---
from pycsg import loadSpaceTreeCSV, OctreeCanvas

# --- Vispy Imports ---
from vispy import app

# -----------------------------------------------------------
data    = loadSpaceTreeCSV("ntree3D.csv")
canvas  = OctreeCanvas(data,boundary=True)

canvas.show()
app.run()