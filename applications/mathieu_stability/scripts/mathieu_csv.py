# --- Python Imports ---
import os

# --- Internal Imports ---
from pycsg import loadSpaceTreeCSV, SpaceTreeCanvas

# --- Vispy Imports ---
from vispy import app

# -----------------------------------------------------------
os.system( "./install/applications/mathieu_stability/mathieu_stability" )
data    = loadSpaceTreeCSV("mathieu_stability.csv")

canvas  = SpaceTreeCanvas(data,boundary=True,visual="point")

canvas.view.camera.rect = (0.0,0.0,10.0,10.0)

canvas.show()
app.run()