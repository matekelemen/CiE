import os
import shutil


# Copy necessary libraries from the parent folder
binaryFileNames     = ('linalg','splinekernel')
libraryExtensions   = ('dll','so')
curDir              = os.path.dirname(os.path.realpath(__file__))
for fileName in binaryFileNames:
    for extension in libraryExtensions:
        name = fileName + '.' + extension
        try:
            shutil.copyfile(os.path.join(curDir,os.pardir,name), os.path.join(curDir,name), follow_symlinks=True)
        except:
            pass
        

from pysplinekernel.surfacekernel import SurfaceKernel
from pysplinekernel.surfacebuilder import SurfaceGUI
from pysplinekernel.editlinevisual import EditLineVisual
from pysplinekernel.canvas import Canvas