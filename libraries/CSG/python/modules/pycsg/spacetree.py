# --- Python Imports ---
import numpy as np
import csv

# --- Visualization Imports ---
from glvisuals import ModularCanvas

# --- Vispy Imports ---
from vispy import scene, app

# -----------------------------------------------------------
def matchSigns(data):
    result = data[0] > 0.0
    for item in data:
        if (item>0.0) is not result:
            return False
    return True


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
    def __init__(self,data,boundary=False):
        ModularCanvas.__init__(self)

        pos     = []
        numVals = len(data) - 2 - 1 - 1
        edgeMin = min(data["length"])

        for index, edgeLength, centerX, centerY in zip(range(len(data["length"])), data["length"],data["center0"],data["center1"]):
            
            if boundary:
                values = [ data["value"+str(vIndex)][index] for vIndex in range(numVals)  ]
                if matchSigns(values) or not np.abs(edgeMin-edgeLength)<1e-10:
                    continue
            
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
    def __init__(self,data,boundary=False,visual="cell"):
        ModularCanvas.__init__(self)

        pos = []
        numVals = len(data) - 3 - 1 - 1
        edgeMin = min(data["length"])

        cell = True
        if visual is "cell":
            cell = True
        elif visual is "point":
            cell = False
        else:
            raise ValueError("Invalid visual type!")

        for index, edgeLength, centerX, centerY, centerZ in zip(range(len(data["length"])), data["length"],data["center0"],data["center1"],data["center2"]):
            
            if boundary:
                values = [ data["value"+str(vIndex)][index] for vIndex in range(numVals)  ]
                if matchSigns(values) or not np.abs(edgeMin-edgeLength)<1e-10:
                    continue
            
            if cell:
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

            else:
                pos.append( [centerX,centerY,centerZ] )

        if cell:
            self.addVisualObject(1,scene.visuals.Line(pos=np.asarray(pos,dtype=np.float32),connect="segments"))
        else:
            self.addVisualObject(1,scene.visuals.Markers(pos=np.asarray(pos,dtype=np.float32)))

        del data

        self.view.camera = scene.ArcballCamera(fov=0)