import glob
import os
import shutil


# Copy necessary libraries from the parent folder
binaryFileNames     = ('linalg.','splinekernel.')
curDir              = os.path.dirname(os.path.realpath(__file__))
parentDir           = os.path.join(curDir,os.pardir)

for fileName in binaryFileNames:
    fileList    = glob.glob(parentDir + '/*' + fileName + '*')
    for filePath in fileList:
	    shutil.copyfile(filePath, os.path.join(curDir,os.path.basename(filePath)))
        

from pysplinekernel.surfacekernel import SurfaceKernel
from pysplinekernel.surfacebuilder import SurfaceGUI
from pysplinekernel.editlinevisual import EditLineVisual
from pysplinekernel.editmarkervisual import EditMarkerVisual
from pysplinekernel.canvas import CanvasSpline2D
#from pysplinekernel.canvas import CanvasSpline2D, CanvasSurface
