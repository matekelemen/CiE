import PyQt5

# --- Internal Imports ---
from pycsg import loadSpaceTreeCSV, SpaceTreeCanvas

# --- Vispy Imports ---
from vispy import app

# -----------------------------------------------------------
data    = loadSpaceTreeCSV("dropmerge.csv")

canvas  = SpaceTreeCanvas(data,boundary=False,visual="cell")

canvas.view.camera.rect = (-2.0,-2.0,4.0,4.0)

canvas.show()
app.run()