import PyQt5

# --- Internal Imports ---
from pycsg import loadSpaceTreeCSV, QuadTreeCanvas

# --- Vispy Imports ---
from vispy import app

# -----------------------------------------------------------
data    = loadSpaceTreeCSV("ntree2D.csv")
canvas  = QuadTreeCanvas(data)

canvas.show()
app.run()