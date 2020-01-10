import PyQt5

# --- Internal Imports ---
from pycsg import loadSpaceTreeCSV, OctreeCanvas

# --- Vispy Imports ---
from vispy import app

# -----------------------------------------------------------
data    = loadSpaceTreeCSV("ntree3D.csv")
canvas  = OctreeCanvas(data)

canvas.show()
app.run()