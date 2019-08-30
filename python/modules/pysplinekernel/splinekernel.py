import numpy as np
from pysplinekernel import pysplinekernel

class SplineKernel:
    # Constructor -------------------------------------------------------------
    def __init__(self):
        self.reset()
        
    # GARBAGE COLLECTION AND RESET --------------------------------------------
    def reset(self):
        self.polynomialOrder = 3
        self.interpolationPoints = [[],[]]
        self.controlPoints = [[],[]]
        self.knotVector = []
        self.samplePoints = []
        self.curvePoints = [[],[]]
        
    # CALCULATION -------------------------------------------------------------
    def getSamplePoints(self):
        return np.linspace(0.0, 1.0, (len(self.interpolationPoints[0])-1)*10 )
    
    def updateSpline(self, lastPoint=[]):
        if lastPoint != []:
            self.controlPoints, self.knotVector = pysplinekernel.interpolateWithBSplineCurve(
                    [self.interpolationPoints[0]+[lastPoint[0]], self.interpolationPoints[1]+[lastPoint[1]]],
                    self.polynomialOrder)
        else:
            self.controlPoints, self.knotVector = pysplinekernel.interpolateWithBSplineCurve(
                    self.interpolationPoints,
                    self.polynomialOrder)
    
    def updatePoints(self):
        x, y = pysplinekernel.evaluate2DCurveDeBoor(
                self.samplePoints,
                self.controlPoints[0],
                self.controlPoints[1],
                self.knotVector)
        return (x,y)
        
    # SET/GET -----------------------------------------------------------------
    def push(self, point):
        if len(self.interpolationPoints[0]) > 0:
            if point[0] != self.interpolationPoints[0][-1] and point[1] != self.interpolationPoints[1][-1]:
                self.interpolationPoints[0].append(point[0])
                self.interpolationPoints[1].append(point[1])
        else:
            self.interpolationPoints[0].append(point[0])
            self.interpolationPoints[1].append(point[1])
        
    def pop(self):
        if len(self.interpolationPoints[0]) > 0:
            self.interpolationPoints[0].pop()
            self.interpolationPoints[1].pop()
    
    def getPoints(self,lastPoint=[]):
        if len(self.interpolationPoints[0]) + len(lastPoint)/2 > self.polynomialOrder:
            self.updateSpline(lastPoint)
            self.samplePoints = self.getSamplePoints()
            self.curvePoints = self.updatePoints()