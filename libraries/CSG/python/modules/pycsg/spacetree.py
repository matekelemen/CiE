# --- Python Imports ---
import numpy as np
import csv

# --- Visualization Imports ---
from glvisuals import ModularCanvas

# --- Vispy Imports ---
from vispy import scene, app

# -----------------------------------------------------------
def loadSpaceTreeCSV(fileName):
    data    = dict()
    header  = []
    with open(fileName) as file:

        reader = csv.reader(file)

        for rowIndex, row in enumerate(reader):

            if rowIndex is 0:
                for item in row:
                    data[item] = []
                header = tuple(data.keys())
            
            else:
                for itemIndex, item in enumerate(row):
                    data[header[itemIndex]].append(float(item))

    return data


class QuadTreeCanvas(ModularCanvas):
    def __init__(self,data):
        ModularCanvas.__init__(self)

        pos = []

        for edgeLength, centerX, centerY in zip(data["length"],data["center0"],data["center1"]):
            radius = edgeLength/2.0
            pos.append( [centerX-radius, centerY-radius, 0.0] )
            pos.append( [centerX+radius, centerY-radius, 0.0] )
            pos.append( [centerX+radius, centerY-radius, 0.0] )
            pos.append( [centerX+radius, centerY+radius, 0.0] )
            pos.append( [centerX+radius, centerY+radius, 0.0] )
            pos.append( [centerX-radius, centerY+radius, 0.0] )
            pos.append( [centerX-radius, centerY+radius, 0.0] )
            pos.append( [centerX-radius, centerY-radius, 0.0] )

        self.addVisualObject(1,scene.visuals.Line(pos=np.asarray(pos,dtype=np.float32),connect="segments"))
        del data


class OctreeCanvas(ModularCanvas):
    def __init__(self,data):
        ModularCanvas.__init__(self)

        pos = []

        for edgeLength, centerX, centerY, centerZ in zip(data["length"],data["center0"],data["center1"],data["center2"]):
            radius = edgeLength/2.0
            pos.append( [centerX-radius, centerY-radius, centerZ-radius] )
            pos.append( [centerX+radius, centerY-radius, centerZ-radius] )
            pos.append( [centerX+radius, centerY-radius, centerZ-radius] )
            pos.append( [centerX+radius, centerY+radius, centerZ-radius] )
            pos.append( [centerX+radius, centerY+radius, centerZ-radius] )
            pos.append( [centerX-radius, centerY+radius, centerZ-radius] )
            pos.append( [centerX-radius, centerY+radius, centerZ-radius] )
            pos.append( [centerX-radius, centerY-radius, centerZ-radius] )

            pos.append( [centerX-radius, centerY-radius, centerZ+radius] )
            pos.append( [centerX+radius, centerY-radius, centerZ+radius] )
            pos.append( [centerX+radius, centerY-radius, centerZ+radius] )
            pos.append( [centerX+radius, centerY+radius, centerZ+radius] )
            pos.append( [centerX+radius, centerY+radius, centerZ+radius] )
            pos.append( [centerX-radius, centerY+radius, centerZ+radius] )
            pos.append( [centerX-radius, centerY+radius, centerZ+radius] )
            pos.append( [centerX-radius, centerY-radius, centerZ+radius] )

            pos.append( [centerX-radius, centerY-radius, centerZ-radius] )
            pos.append( [centerX-radius, centerY-radius, centerZ+radius] )
            pos.append( [centerX-radius, centerY+radius, centerZ-radius] )
            pos.append( [centerX-radius, centerY+radius, centerZ+radius] )
            pos.append( [centerX+radius, centerY-radius, centerZ-radius] )
            pos.append( [centerX+radius, centerY-radius, centerZ+radius] )
            pos.append( [centerX+radius, centerY+radius, centerZ-radius] )
            pos.append( [centerX+radius, centerY+radius, centerZ+radius] )

        self.addVisualObject(1,scene.visuals.Line(pos=np.asarray(pos,dtype=np.float32),connect="segments"))
        del data

        self.view.camera = scene.ArcballCamera(fov=0)