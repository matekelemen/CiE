import glob
import os
import shutil

'''
# Copy necessary libraries from the parent folder
binaryFileNames     = ('linalg.','splinekernel.')
curDir              = os.path.dirname(os.path.realpath(__file__))
parentDir           = os.path.join(curDir,os.pardir)

for fileName in binaryFileNames:
    fileList    = glob.glob(parentDir + '/*' + fileName + '*')
    for filePath in fileList:
	    shutil.copyfile(filePath, os.path.join(curDir,os.path.basename(filePath)))
'''
        
from .pysplinekernel import *
from .surfacekernel import SurfaceKernel
from .editlinevisual import EditLineVisual
from .editmarkervisual import EditMarkerVisual
from .canvas import CanvasSpline2D