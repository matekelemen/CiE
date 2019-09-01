import sys
from vispy import app, scene
from PyQt5 import QtCore, QtWidgets
from pysplinekernel.app import MainWindow

appQt = QtWidgets.QApplication(sys.argv)
win = MainWindow()
win.show()
app.run()

'''
# IMPORT ----------------------------------------------------------
import sys
import numpy as np

from vispy import app, scene

import pysplinekernel
from vispy.geometry.generation import create_sphere
from vispy.color.colormap import get_colormaps

try:
    from sip import setapi
    setapi("QVariant", 2)
    setapi("QString", 2)
except ImportError:
    pass

from PyQt5 import QtWidgets, QtCore

# EMBED VISPY IN QT -----------------------------------------------
class ObjectWidget(QtWidgets.QWidget):
    """
    Widget for editing OBJECT parameters
    """
    signal_object_changed = QtCore.pyqtSignal(name='objectChanged')

    def __init__(self, parent=None):
        super(ObjectWidget, self).__init__(parent)

        labelPolynomialOrder = QtWidgets.QLabel("Polynomial order")
        self.controlPolynomialOrder = QtWidgets.QSpinBox()
        self.controlPolynomialOrder.setMinimum(1)
        self.controlPolynomialOrder.setMaximum(10)
        self.controlPolynomialOrder.setValue(3)
        self.controlPolynomialOrder.valueChanged.connect(self.updateParam)

        gbox = QtWidgets.QGridLayout()
        gbox.addWidget(labelPolynomialOrder, 0, 0)
        gbox.addWidget(self.controlPolynomialOrder, 0, 1)

        vbox = QtWidgets.QVBoxLayout()
        vbox.addLayout(gbox)
        vbox.addStretch(1.0)

        self.setLayout(vbox)

    def updateParam(self, option):
        self.signal_object_changed.emit()


class MainWindow(QtWidgets.QMainWindow):

    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)

        self.resize(1024, 768)
        self.setWindowTitle('Spline Builder')

        splitter = QtWidgets.QSplitter(QtCore.Qt.Horizontal)

        self.canvas = pysplinekernel.Canvas()
        self.canvas.create_native()
        self.canvas.native.setParent(self)

        #self.props = ObjectWidget()
        #splitter.addWidget(self.props)
        #splitter.addWidget(self.canvas.native)

        #self.setCentralWidget(splitter)
        #self.props.signal_object_changed.connect(self.update_view)
        #self.update_view()

    def update_view(self):
        # banded, nbr_steps, cmap
        pass
        #self.canvas.set_data(self.props.nbr_steps.value(),
        #                     self.props.combo.currentText())


# -----------------------------------------------------------------------------
if __name__ == '__main__':
    app.run()
    appQt = QtWidgets.QApplication(sys.argv)
    win = MainWindow()
    win.show()
    appQt.exec_()
'''