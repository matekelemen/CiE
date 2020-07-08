# --- Discretization Imports ---
from pyfem.discretization.element import Element1D
from pyfem.discretization import TransientFEModel, NonlinearTransientFEModel

# --- Python Imports ---
import numpy as np
import scipy.sparse as sparse

# ---------------------------------------------------------
class AdjointHeatElement1D( Element1D ):
    '''
    Element type for AdjointModel
    The coefficients (capacity, conductivity) don't depend on the solution directly,
    but on the position, so integration must be carried out accordingly.

    The constructor expects a NonlinearHeatElement1D and a forward solution, from which
    the coefficients are constructed. An extra integration target (nonSymmetricStffness)
    is added.
    '''
    def __init__(   self, 
                    nonlinearHeatElement1D, 
                    forwardModel, 
                    forwardSolution, 
                    referenceSolution,
                    ddConductivity      # Second derivative of the original element's conductivity
                                        # TODO: create a wrapper function for these coefficients that
                                        #       encapsulate their derivatives to avoid this clutter
                     ):
        '''
        nonlinearHeatElement1D  : element from the forward model
        model                   : forward model of type NonlinearTransientFEModel
        forwardSolution         : list of coefficients representing the temperature field u(t,x)
                                    for the specified time points
        '''
        super( AdjointHeatElement1D, self ).__init__(   nonlinearHeatElement1D.domain,
                                                        nonlinearHeatElement1D.DoFs,
                                                        nonlinearHeatElement1D.load,
                                                        basisFunctions=nonlinearHeatElement1D.basisFunctions,
                                                        integrationOrder=nonlinearHeatElement1D.integrator.polynomialOrder )
        self.element            = nonlinearHeatElement1D
        self.solutionField      = forwardModel.sample
        self.derivativeField    = forwardModel.sampleDerivative
        self.forwardSolution    = forwardSolution

        # Second derivative of the original element's conductivity
        # TODO: create a wrapper function for these coefficients that
        #       encapsulate their derivatives
        self._ddConductivity    = ddConductivity

        # TODO: Caching


    def capacity( self, timeIndex, x ):
        u = self.solutionField( self.forwardSolution[timeIndex], x )
        return self.element.capacity(u)*u + self.element.capacityDerivative(u)


    def conductivity( self, timeIndex, x ):
        u = self.solutionField( self.forwardSolution[timeIndex], x )
        return self.element.conductivity(u) + self.element.conductivityDerivative(u)


    def conductivityDerivative( self, timeIndex, x ):
        u   = self.solutionField( self.forwardSolution[timeIndex], x )
        du  = self.derivativeField( self.forwardSolution[timeIndex], x )
        return ( self.element.conductivityDerivative(u) + self._ddConductivity(u) ) * du


    def integrateNonsymmetricStiffness( self, globalMatrix ):
        '''
        Integrate conductivityDerivative * Ni*dNj
        '''
        basisCache                  = [ self.integrator.createCache( self.basisFunctions[i], self.basisFunctions.domain ) for i in range(len(self.basisFunctions)) ]
        basisDerivativeCache        = [ self.integrator.createCache( self.basisDerivatives[i], self.basisDerivatives.domain ) for i in range(len(self.basisDerivatives)) ]
        conductivityDerivativeCache = [ self.integrator.createCache( lambda xi: self.conductivityDerivative(self.toGlobalCoordinates(xi)), self.basisFunctions.domain ) ]

        for i in range(len(self.basisFunctions)):
            cache = conductivityDerivativeCache * basisCache[i]
            for j in range(len(self.basisDerivatives)):
                value = self.integrator.integrateCached(    lambda x: 1.0,
                                                            cache * basisDerivativeCache[j],
                                                            self.basisFunctions.domain )
                globalMatrix[self.DoFs[i],self.DoFs[j]] += value


class AdjointModel(TransientFEModel):
    '''
    Nonlinear adjoint model attached to a NonlinearTransientFEModel
    '''
    def __init__( self, model, time, referenceTimeSeries, forwardTimeSeries, *args, **kwargs ):
        TransientFEModel.__init__( self, *args, **kwargs )

        # Check model type
        # The adjoint system is linear with respect to its state, the system
        # it's derived from must be nonlinear. A simpler structure is used
        # for solving linear systems.
        assert type(model) is NonlinearTransientFEModel, "Invalid model type!"

        self.femodel                = model
        self.time                   = time
        self.referenceSolution      = referenceTimeSeries
        self.forwardSolution        = forwardTimeSeries

        self._loadFunction          = self.adjointLoadFunctor

        # Allocate matrix for non-symmetric stiffness
        DoFs                        = np.transpose( self.getDoFs(), (1,0) )
        self.nonsymmetricStiffness  = sparse.csr_matrix(    ( np.zeros( DoFs.shape[1] ), DoFs ), 
                                                            shape=self.shape )
        
        # Transform model members 
        # TODO: ddConductivity
        for element in model.elements:
            self.elements.append(
                AdjointHeatElement1D(   element,
                                        self.femodel,
                                        self.forwardSolution,
                                        self.referenceSolution,
                                        lambda x: 0)
            )