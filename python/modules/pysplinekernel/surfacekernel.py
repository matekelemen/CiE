from pysplinekernel import pysplinekernel


# -----------------------------------------------------------------------------
# CLASS DEFINITIONS
# -----------------------------------------------------------------------------
class SurfaceKernel:
    # Constructor -------------------------------------------------------------
    def __init__(self,interpolationPoints=[[],[],[]], meshSize=[0,0], polynomialOrders=[3,3]):
        # Reset
        self.reset()

        # Set interpolation points
        self.setInterpolationPoints(interpolationPoints=interpolationPoints,meshSize=meshSize)

        # Set polynomial order
        self.setPolynomialOrder(polynomialOrders=polynomialOrders)
        

    # GARBAGE COLLECTION AND RESET --------------------------------------------
    def reset(self):
        self.meshSize               = [0,0]
        self.samples                = [5,5]
        self.polynomialOrders       = [3,3]
        self.interpolationPoints    = [[],[],[]]
        self.controlPoints          = [[],[],[]]
        self.knotVectors            = [[],[]]


    # MISC ------------------------------------------------------------
    def getIndicesInAxis(self,axis=0,axisIndex=0):
        # Get indices of all points on the given axis
        # Example: getIndices(axis=1, axisIndex=5) returns the indices of all points in the 5th column
        if axis is 0:
            indices = [ x+axisIndex*self.meshSize[0] for x in range(0,self.meshSize[0]) ]
        elif axis is 1:
            indices = [ x+axisIndex for x in range(0,len(self.interpolationPoints[1]),self.meshSize[0]) ]
        else:
            raise(ValueError('Invalid axis index'))
        return indices


    def findAxisMatch(self, value, axis):
        # Check if value is already present in axis
        # Example: findAxisMatch(3.14, 0) returns true if there is already an interpolation point with an x value of 3.14
        if len(self.interpolationPoints[axis]) > 0:
            coordinates = [ self.interpolationPoints[axis][x] for x in self.getIndicesInAxis(axis=axis) ]
            try:
                return next( (i for i,x in enumerate(coordinates) if abs(x-value)<1e-15) )
            except: 
                return None
        else:
            return None


    def findPlaceInAxis(self, value, axis):
        # Return the index of the first component in axis greater than value
        if len(self.interpolationPoints[axis]) > 0:
            coordinates = [ self.interpolationPoints[axis][x] for x in self.getIndicesInAxis(axis=axis) ]
            try:
                return next( (i for i,x in enumerate(coordinates) if x > value) )
            except:
                return len(coordinates)
        else:
            return 0
        

    # CALCULATION -------------------------------------------------------------
    def interpolate(self):
        self.controlPoints, self.knotVectors = pysplinekernel.interpolateWithBSplineSurface(
                self.interpolationPoints,
                self.polynomialOrders[0],
                self.polynomialOrders[1])
        

    # SET/GET -----------------------------------------------------------------
    def setPolynomialOrder(self,polynomialOrders=(3,3)):
        if polynomialOrders[0]>0 and polynomialOrders[1]>0:
            if type(polynomialOrders[0]) is int and type(polynomialOrders[1]) is int:
                self.polynomialOrders = polynomialOrders
            else:
                raise(ValueError('Invalid polynomial order'))
        else:
            raise(ValueError('Invalid polynomial order'))


    def setInterpolationPoints(self,interpolationPoints=[[],[],[]],meshSize=[0,0]):
        if meshSize[0]*meshSize[1] is len(interpolationPoints[0]):
            if len(interpolationPoints)is 3 and len(interpolationPoints[0]) is len(interpolationPoints[1]) is len(interpolationPoints[2]):
                self.interpolationPoints    = interpolationPoints
                self.meshSize               = meshSize
            else:
                raise(ValueError('Number of components in interpolationPoints must match!'))
        else:
            raise(ValueError('Invalid mesh size!'))


    def addPoint(self,point):
        # Check if components match with any existing ones
        xMatch  = self.findAxisMatch( point[0], 0 )
        yMatch  = self.findAxisMatch( point[1], 1 )
        if xMatch and yMatch:
            # Do nothing if the point is already stored
            return False
        # Find column index
        columnIndex = self.findPlaceInAxis(point[0],0)
        rowIndex    = self.findPlaceInAxis(point[1],1)
        # Create indices and coordinates
        offset  = columnIndex

        indices = [ x+i+offset for i,x in enumerate( iR*self.meshSize[0] for iR in range(rowIndex) ) ]
        cX      = [ point[0] for x in range(rowIndex) ]
        cY      = self.interpolationPoints[1][:rowIndex*self.meshSize[0]:self.meshSize[0]]
        offset  += len(indices)

        indices.extend( list(range( rowIndex*(self.meshSize[0]+1),(rowIndex+1)*(self.meshSize[0]+1) )) )
        temp    = self.interpolationPoints[0][:self.meshSize[0]]
        temp[columnIndex:columnIndex] = [point[0]]
        cX.extend( temp )
        cY.extend( [point[1] for x in range(self.meshSize[0]+1)] )
        offset  += self.meshSize[0]+1

        indices.extend(
            [ x+i+offset for i,x in enumerate( iR*self.meshSize[0] for iR in range(rowIndex,self.meshSize[1]) ) ]
        )
        cX.extend( [point[0] for x in range(rowIndex,self.meshSize[1])] )
        cY.extend( self.interpolationPoints[1][rowIndex*self.meshSize[0]::self.meshSize[0]] )
        # Add points
        offset          = 0
        for index,x,y in zip(indices,cX,cY):
            if xMatch and abs(x-xMatch)<1e-15:
                offset-=1
                continue
            elif yMatch and abs(y-yMatch)<1e-15:
                offset-=1
                continue
            self.interpolationPoints[0][index+offset:index+offset] = [x]
            self.interpolationPoints[1][index+offset:index+offset] = [y]
            self.interpolationPoints[2][index+offset:index+offset] = [0.0]
        # Update mesh size
        if not xMatch:
            self.meshSize[0] += 1
        if not yMatch:
            self.meshSize[0] += 1

    # SURFACE GENERATION ------------------------------------------------------
    def generatePoints(self):
        self.interpolate()
        return pysplinekernel.evaluateSurface(
                self.knotVectors,
                self.controlPoints,
                self.samples)