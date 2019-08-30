from pysplinekernel import pysplinekernel
import numpy as np
import matplotlib
matplotlib.use('qt5agg')
import matplotlib.pyplot as plt


# -----------------------------------------------------------------------------
# CLASS DEFINITIONS
# -----------------------------------------------------------------------------
class SurfaceKernel:
    # Constructor -------------------------------------------------------------
    def __init__(self,interpolationPoints=[[],[],[]]):
        self.reset()
        if interpolationPoints != [[],[],[]]:
            self.interpolationPoints = interpolationPoints
        
    # GARBAGE COLLECTION AND RESET --------------------------------------------
    def reset(self):
        self.samples = [5,5]
        self.polynomialOrders = [3,3]
        self.interpolationPoints = [[],[],[]]
        self.controlPoints = [[],[],[]]
        self.knotVectors = [[],[]]
        self.samplePoints = [[],[]]
        self.surfacePoints = [[],[],[]]
        
    # CALCULATION -------------------------------------------------------------
    def updateSurface(self, lastPoint=[]):
        if lastPoint != []:
            self.controlPoints, self.knotVectors = pysplinekernel.interpolateWithBSplineSurface(
                    [self.interpolationPoints[0]+[lastPoint[0]], self.interpolationPoints[1]+[lastPoint[1]], self.interpolationPoints[2]+[lastPoint[2]]],
                    self.polynomialOrders[0],
                    self.polynomialOrders[1])
        else:
            self.controlPoints, self.knotVectors = pysplinekernel.interpolateWithBSplineSurface(
                    self.interpolationPoints,
                    self.polynomialOrders[0],
                    self.polynomialOrders[1])
    
    def updatePoints(self):
        self.surfacePoints = pysplinekernel.evaluateSurface(
                self.knotVectors,
                self.controlPoints,
                self.samples)
        
    # SET/GET -----------------------------------------------------------------
    def generatePoints(self,lastPoint=[]):
        self.updateSurface(lastPoint)
        self.updatePoints()
        
