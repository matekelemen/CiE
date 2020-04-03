# --- Python Imports ---
import numpy as np

# --- Internal Imports ---
from .basisfunctions import *
from pyfem.numeric import Integrator
from pyfem.utilities import isArray

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
        self.domain             = np.asarray( domain )
        self.DoFs               = np.asarray( DoFs )
        self.load               = load
        self.integrator         = Integrator(integrationOrder)

        self.basisDerivatives   = basisFunctions.derivatives()


    def updateGlobalMatrix( self, matrix, index1, index2, value ):
        matrix[self.DoFs[index1], self.DoFs[index2]] += value
        if index1 != index2:
            matrix[self.DoFs[index2], self.DoFs[index1]] += value


    def updateGlobalVector( self, vector, index, value ):
        vector[self.DoFs[index]] += value


    def integrateStiffness( self, globalStiffnessMatrix ):
        raise NotImplementedError( "Attempt to call abstract base class function" )


    def integrateMass( self, globalMassMatrix ):
        raise NotImplementedError( "Attempt to call abstract base class function" )


    def integrateLoad( self, globalLoadVector ):
        raise NotImplementedError( "Attempt to call abstract base class function" )



class Element1D( Element ):
    '''
    '''
    def __init__( self, *args, **kwargs ):
        Element.__init__( self, *args, **kwargs )
        
        # Check domain
        if self.domain[1] <= self.domain[0]:
            self.domain = np.flip( self.domain )

        # Jacobian ( dx/dxi )
        self._jacobian      = (self.domain[1]-self.domain[0])/2.0
        self._invJacobian   = 1.0/self._jacobian


    def toGlobalCoordinates( self, localCoordinates ):
        '''
        [-1,1] -> self.domain
        '''
        if isArray( localCoordinates ):
            return np.asarray( [ self._jacobian * (xi + 1.0) + self.domain[0] for xi in localCoordinates ] )
        else:
            return self._jacobian * (localCoordinates + 1.0) + self.domain[0]


    def toLocalCoordinates( self, globalCoordinates ):
        '''
        self.domain -> [-1,1]
        '''
        if isArray( globalCoordinates ):
            return np.asarray( [ (x-self.domain[0]) * self._invJacobian - 1.0 for x in globalCoordinates ] )
        else:
            return (globalCoordinates-self.domain[0]) * self._invJacobian - 1.0


    def __call__( self, coefficients, positions ):
        '''
        Given the solution coefficients, sample and sum the basis functions at the given coordinates
        '''
        # Initialize
        values = None
        if isNumpyArray( positions ):
            values = np.zeros( positions.shape, positions.dtype )
        else:
            values = 0.0

        # Compute solution
        for basisID, coefficient in enumerate( coefficients ):
            values += coefficient * self.basisFunctions( basisID, self.toLocalCoordinates(positions) )
            
        return values



class LinearHeatElement1D( Element1D ):
    '''
    Most basic element for the Poisson equation
    '''
    def __init__( self, capacity, conductivity, *args, **kwargs ):
        Element1D.__init__(self, *args, **kwargs )
        self.capacity       = capacity
        self.conductivity   = conductivity

    
    def integrateStiffness( self, globalStiffnessMatrix ):
        for i, function in enumerate(self.basisDerivatives):
            for j in range( i, len(self.basisDerivatives) ):
                value = self.conductivity * self._invJacobian * self.integrator( lambda x: function(x)*self.basisDerivatives[j](x), self.basisDerivatives.domain )
                self.updateGlobalMatrix( globalStiffnessMatrix, i, j, value )


    def integrateMass( self, globalMassMatrix ):
        for i, function in enumerate(self.basisFunctions):
            for j in range( i, len(self.basisFunctions) ):
                value = self.capacity * self._jacobian * self.integrator( lambda x: function(x)*self.basisFunctions[j](x), self.basisFunctions.domain )
                self.updateGlobalMatrix( globalMassMatrix, i, j, value )


    def integrateLoad( self, globalLoadVector ):
        for i, function in enumerate(self.basisFunctions):
            value = self._jacobian * self.integrator( lambda x: function(x) * self.load(self.toGlobalCoordinates(x)), self.basisFunctions.domain )
            self.updateGlobalVector( globalLoadVector, i, value )



class NonlinearHeatElement1D( Element1D ):
    '''
    Basic heat element with temperature-dependent coefficients
    '''
    def __init__( self, capacity, capacityDerivative, conductivity, conductivityDerivative, *args, **kwargs ):
        Element1D.__init__( self, *args, **kwargs )
        if not callable(capacity) or not callable(conductivity):
            raise AttributeError( "The coefficients of NonlinearHeatElement1D must be callable (functions of temperature)" )

        self.capacity               = capacity
        self.capacityDerivative     = capacityDerivative
        self.conductivity           = conductivity
        self.conductivityDerivative = conductivityDerivative


    def integrateStiffness( self, globalStiffnessMatrix, temperatureFunction, solution ):
        for i in range(len(self.basisDerivatives)):
            dNi      = self.basisDerivatives[i]
            for j in range( i, len(self.basisDerivatives) ):
                dNj     = self.basisDerivatives[j]
                value   = self._invJacobian * self.integrator(  lambda x: self.conductivity(temperatureFunction(self.toGlobalCoordinates(x))) * dNi(x)*dNj(x), 
                                                                self.basisDerivatives.domain )
                self.updateGlobalMatrix( globalStiffnessMatrix, i, j, value )


    def integrateMass( self, globalMassMatrix, temperatureFunction, solution ):
        for i in range(len(self.basisFunctions)):
            Ni      = self.basisFunctions[i]
            for j in range( i, len(self.basisFunctions) ):
                Nj      = self.basisFunctions[j]
                value   = self._jacobian * self.integrator(   lambda x: self.capacity(temperatureFunction(self.toGlobalCoordinates(x))) * Ni(x)*Nj(x), 
                                                            self.basisFunctions.domain )
                self.updateGlobalMatrix( globalMassMatrix, i, j, value )


    def integrateLoad( self, globalLoadVector, temperatureFunction, solution ):
        for i in range(len(self.basisFunctions)):
            Ni      = self.basisFunctions[i]
            value   = self._jacobian * self.integrator( lambda x: Ni(x) * self.load(self.toGlobalCoordinates(x)), self.basisFunctions.domain )
            self.updateGlobalVector( globalLoadVector, i, value )


    def integrateGeometricStiffness( self, matrix, temperatureFunction, solution ):
        for i in range(len( self.basisDerivatives )):
            dNi         = self.basisDerivatives[i]
            for j in range(len( self.basisFunctions )):
                Nj     = self.basisFunctions[j]
                value   = 0.0
                for k in range(len( self.basisDerivatives )):
                    dNk     = self.basisDerivatives[k]
                    value   +=  self._invJacobian * self.integrator(  lambda x: self.conductivityDerivative(self.toGlobalCoordinates(x)) * dNi(x)*dNk(x)*Nj(x),
                                                                self.basisFunctions.domain )    \
                                * solution[self.DoFs[k]]
                matrix[ self.DoFs[i], self.DoFs[j] ] += value
                