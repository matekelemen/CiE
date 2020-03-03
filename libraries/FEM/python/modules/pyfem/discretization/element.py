# --- Python Imports ---
import numpy as np

# --- Internal Imports ---
from .basisfunctions import *
from pyfem.numeric import Integrator

# ---------------------------------------------------------

class Element:
    '''
    Element base class
    '''
    def __init__( self, domain, DoFs, load, basisFunctions=LinearBasisFunctions(), integrationOrder=2 ):
        # Check degrees of freedom
        if len(DoFs) is not len(basisFunctions._functions):
            raise AttributeError( "Number of degrees of freedom must match" )

        # Set members
        self.basisFunctions     = basisFunctions
        self.domain             = domain
        self.DoFs               = DoFs
        self.load               = load
        self.integrator         = Integrator(integrationOrder)

        self.basisDerivatives   = basisFunctions.derivatives()


    def updateGlobalMatrix( self, matrix, index1, index2, value ):
        matrix[self.DoFs[index1]][self.DoFs[index2]] += value
        if index1 is not index2:
            matrix[self.DoFs[index1]][self.DoFs[index2]] += value


    def updateGlobalVector( self, vector, index, value ):
        vector[index] += value



class LinearHeatElement1D( Element ):
    '''
    Most basic element for the Poisson equation
    '''
    def __init__( self, capacity, conductivity, *args, **kwargs ):
        Element.__init__(self, *args, **kwargs )
        self.capacity       = capacity
        self.conductivity   = conductivity

    
    def integrateStiffness( self, globalStiffnessMatrix ):
        for i, function in enumerate(self.basisDerivatives):
            for j in range( i, len(self.basisDerivatives) ):
                value = self.conductivity * 2.0/(self.domain[1]-self.domain[0]) * self.integrator( lambda t: function(t)*self.basisDerivatives[j](t), function.domain )
                self.updateGlobalMatrix( globalStiffnessMatrix, i, j, value )


    def integrateMass( self, globalMassMatrix ):
        for i, function in enumerate(self.basisFunctions):
            for j in range( i, len(self.basisFunctions) ):
                value = self.capacity * (self.domain[1]-self.domain[0])/2.0 * self.integrator( lambda t: function(t)*self.basisFunctions[j](t), function.domain )
                self.updateGlobalMatrix( globalMassMatrix, i, j, value )


    def integrateLoad( self, globalLoadVector ):
        self.conductivity