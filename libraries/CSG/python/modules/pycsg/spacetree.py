# --- Python Imports ---
import numpy as np
import csv
from timeit import default_timer

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

        for index, edgeLength, x, y in zip(range(len(data["length"])), data["length"],data["center0"],data["center1"]):
            
            if boundary:
                values = [ data["value"+str(vIndex)][index] for vIndex in range(numVals)  ]
                if matchSigns(values) or not np.abs(edgeMin-edgeLength)<1e-10:
                    continue
            
            radius = edgeLength/2.0
            pos.append( [x-radius, y-radius, 0.0] )
            pos.append( [x+radius, y-radius, 0.0] )
            pos.append( [x+radius, y-radius, 0.0] )
            pos.append( [x+radius, y+radius, 0.0] )
            pos.append( [x+radius, y+radius, 0.0] )
            pos.append( [x-radius, y+radius, 0.0] )
            pos.append( [x-radius, y+radius, 0.0] )
            pos.append( [x-radius, y-radius, 0.0] )

        self.addVisualObject(1,scene.visuals.Line(pos=np.asarray(pos,dtype=np.float32),connect="segments"))
        del data


class OctreeCanvas(ModularCanvas):
    def __init__(   self,
                    data,
                    boundary=False,
                    visual="cell",
                    animation=None):
        ModularCanvas.__init__(self)

        self.unfreeze()

        self.dimensions = len(data["center"][0])
        self.boundary   = boundary
        self.visual     = visual
        self.animation  = animation
        self._timer     = app.Timer('auto', connect=self.on_timer, start='true')
        self.t0         = default_timer()
        self.t          = 0

        pos             = np.empty( (1,3), dtype=np.float32 )
        if self.visual is "cell" and self.dimensions > 1:
            self.addVisualObject(0,scene.visuals.Line(  pos=np.asarray(pos,dtype=np.float32),
                                                        connect="segments"))
        elif self.visual is "point":
            self.addVisualObject(0,scene.visuals.Markers(   pos=np.asarray(pos,dtype=np.float32),
                                                            size=3))
        else:
            raise ValueError("Invalid visual type!")

        self.updateData(data)

        if self.dimensions > 2:
            self.view.camera = scene.ArcballCamera(fov=0)

        self.freeze()


    def updateData(self,data):
        pos = []
        for boundary, edgeLength, center in zip( data["boundary"], data["length"], data["center"] ):
            
            if self.boundary:
                if boundary is 0:
                    continue

            x = center[0]
            y = 0.0
            z = 0.0

            if self.dimensions > 1:
                y = center[1]
            if self.dimensions > 2:
                z = center[2]
            
            if self.visual is "cell":
                radius = edgeLength/2.0
                pos.append( [x-radius, y-radius, z-radius] )
                pos.append( [x+radius, y-radius, z-radius] )
                pos.append( [x+radius, y-radius, z-radius] )
                pos.append( [x+radius, y+radius, z-radius] )
                pos.append( [x+radius, y+radius, z-radius] )
                pos.append( [x-radius, y+radius, z-radius] )
                pos.append( [x-radius, y+radius, z-radius] )
                pos.append( [x-radius, y-radius, z-radius] )

                pos.append( [x-radius, y-radius, z+radius] )
                pos.append( [x+radius, y-radius, z+radius] )
                pos.append( [x+radius, y-radius, z+radius] )
                pos.append( [x+radius, y+radius, z+radius] )
                pos.append( [x+radius, y+radius, z+radius] )
                pos.append( [x-radius, y+radius, z+radius] )
                pos.append( [x-radius, y+radius, z+radius] )
                pos.append( [x-radius, y-radius, z+radius] )

                pos.append( [x-radius, y-radius, z-radius] )
                pos.append( [x-radius, y-radius, z+radius] )
                pos.append( [x-radius, y+radius, z-radius] )
                pos.append( [x-radius, y+radius, z+radius] )
                pos.append( [x+radius, y-radius, z-radius] )
                pos.append( [x+radius, y-radius, z+radius] )
                pos.append( [x+radius, y+radius, z-radius] )
                pos.append( [x+radius, y+radius, z+radius] )

            else:
                pos.append( [x,y,z] )
                

        if self.visual is "cell":
            self.objects[0].set_data( pos=np.asarray(pos,dtype=np.float32) )
        else:
            self.objects[0].set_data( pos=np.asarray(pos,dtype=np.float32), size=3 )


    def on_timer(self,event):
        if self.animation is not None:
            self.t = 2.0 - (default_timer() - self.t0) / 10.0
            if self.t < 2.0 and self.t > 0.0:
                self.animation(self)
            else:
                app.quit()