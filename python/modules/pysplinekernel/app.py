# IMPORTS ----------------------------------------------------------------
from PyQt5.QtWidgets import *
from PyQt5 import QtCore
import numpy as np
from vispy import app, scene
import sys

import pysplinekernel
# ------------------------------------------------------------------------

class ControlWidget(QWidget):
    def __init__(self, parent=None):
        super(ControlWidget, self).__init__(parent)

        labelPolynomialOrder = QLabel("Polynomial order")
        self.controlPolynomialOrder = QSpinBox()
        self.controlPolynomialOrder.setMinimum(1)
        self.controlPolynomialOrder.setMaximum(10)
        self.controlPolynomialOrder.setValue(3)

        gbox = QGridLayout()
        gbox.addWidget(labelPolynomialOrder, 0, 0)
        gbox.addWidget(self.controlPolynomialOrder, 0, 1)

        vbox = QVBoxLayout()
        vbox.addLayout(gbox)
        vbox.addStretch(1.0)

        self.setLayout(vbox)

class MainWindow(QMainWindow):

    def __init__(self):
        QMainWindow.__init__(self)

        self.resize(1024, 768)
        self.setWindowTitle('Spline Builder')

        splitter = QSplitter(QtCore.Qt.Horizontal)

        self.canvas = pysplinekernel.Canvas()
        self.canvas.create_native()
        self.canvas.native.setParent(self)

        self.controlWidget = ControlWidget()
        splitter.addWidget(self.controlWidget)
        splitter.addWidget(self.canvas.native)

        self.setCentralWidget(splitter)