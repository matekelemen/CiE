import sys
from vispy import app, scene
from PyQt5 import QtCore, QtWidgets
from pysplinekernel.appSpline2D import MainWindow

appQt = QtWidgets.QApplication(sys.argv)
win = MainWindow()
win.show()
app.run()