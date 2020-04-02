# --- Python Imports ---
import numpy as np
import scipy.sparse as sparse
import functools
from copy import copy

# --- Internal Imports ---
from pyfem.utilities import isNumpyArray, gridPairs
from .element import Element, Element1D

# ---------------------------------------------------------
# Decorators
def requiresInitialized( function ):
    @functools.wraps( function )
    def decoratedFunction( instance, *args, **kwargs ):
        for value in vars(instance).values():
            if value is None:
                raise AttributeError( "Uninitialized member variable when calling a function that requires an initialized object!" )
        else:
            return function( instance, *args, **kwargs )
    return decoratedFunction

# ---------------------------------------------------------
class FEModel:
    '''
    Aggregate base class to hold elements, store global matrices, and apply constraints to
    '''
    def __init__( self, size ):
        self.size   = size
        self.shape  = np.asarray( (size, size), dtype=np.uint64 )

        # Initialize elements
        self.elements       = None

        # Initialize matrices
        self.stiffness      = None
        self.load           = None

        self.boundaries     = []


    def resetMatrices( self ):
        # Reset structural matrices
        self.stiffness.data = np.zeros( self.stiffness.data.shape, dtype=self.stiffness.data.dtype )
        self.load           = np.zeros( self.load.shape, dtype=self.load.dtype )

        # Reset boundaries
        for boundary in self.boundaries:
            boundary.applied = False


    def getDoFs( self ):
        '''
        Get a unique set of DoF pairs that indicate nonzero entries in matrices
        '''
        if self.elements is None:
            raise AttributeError( "Attempt to get DoFs of an uninitialized model" )

        DoFs = None
        for element in self.elements:
            if DoFs is None:
                DoFs = gridPairs( element.DoFs )
            else:
                DoFs = np.concatenate( (DoFs, gridPairs( element.DoFs )), axis=0 )

        return np.unique( DoFs, axis=0 )


    def allocateZeros( self ):
        # Get nonzero indices
        DoFs = np.transpose( self.getDoFs(), (1,0) )

        # Check if already initialized
        if self.stiffness is not None or self.load is not None:
            raise RuntimeError( "Attempt to initialize existing matrices" )

        # Initialize matrices
        self.stiffness  = sparse.csr_matrix(    ( np.zeros( DoFs.shape[1] ), DoFs ), 
                                                shape=self.shape )
        self.load       = np.zeros( self.size )

        return DoFs


    @requiresInitialized
    def integrate( self ):
        '''
        Loop through elements and integrate their matrices
        '''
        for element in self.elements:
            element.integrateStiffness( self.stiffness )
            element.integrateLoad( self.load )


    def addBoundaryCondition( self, boundaryCondition, *args, **kwargs ):
        self.boundaries.append( boundaryCondition )
        self.applyBoundaryCondition( boundaryCondition, *args, **kwargs )
        return len(self.boundaries)-1


    def removeBoundaryCondition( self, boundaryID ):
        boundaryCondition       = self.boundaries.pop( boundaryID )
        if boundaryCondition.BCType is "dirichlet":
            self.removeDirichletBoundary( boundaryCondition )
        elif boundaryCondition.BCType is "neumann":
            self.removeNeumannBoundary( boundaryCondition )
        else:
            raise NotImplementedError( "Unknown boundary condition type" )


    @requiresInitialized
    def applyBoundaryCondition( self, boundaryCondition, *args, **kwargs ):
        if boundaryCondition.BCType is "dirichlet":
            self.applyDirichletBoundary( boundaryCondition, *args, **kwargs )
        elif boundaryCondition.BCType is "neumann":
            self.applyNeumannBoundary( boundaryCondition, *args, **kwargs )
        else:
            raise NotImplementedError( "Unknown boundary condition type" )


    @requiresInitialized
    def applyDirichletBoundary( self, boundaryCondition ):
        #if boundaryCondition.applied is not True:
        #    for index in range(self.size):
        #        if np.abs( self.stiffness[boundaryCondition.DoF, index] )>1e-15:
        #            self.stiffness[boundaryCondition.DoF, index] = 0.0
        #        if np.abs( self.stiffness[ index, boundaryCondition.DoF] )>1e-15:
        #            self.stiffness[ index, boundaryCondition.DoF ] = 0.0
        #    #self.stiffness[boundaryCondition.DoF,boundaryCondition.DoF] += boundaryCondition.penaltyValue
        #    self.stiffness[boundaryCondition.DoF,boundaryCondition.DoF] = 1.0
        #    boundaryCondition.applied = True
        ##self.load[boundaryCondition.DoF]    += boundaryCondition.penaltyValue * boundaryCondition.value
        #self.load[boundaryCondition.DoF] = boundaryCondition.value
        self.stiffness[boundaryCondition.DoF, boundaryCondition.DoF] += boundaryCondition.penaltyValue
        self.load[boundaryCondition.DoF] += boundaryCondition.penaltyValue * boundaryCondition.value


    @requiresInitialized
    def removeDirichletBoundary( self, boundaryCondition ):
        if boundaryCondition.applied is not True:
            self.stiffness[boundaryCondition.DoF,boundaryCondition.DoF] -= boundaryCondition.penaltyValue
            boundaryCondition.applied = False
        self.load[boundaryCondition.DoF]    -= boundaryCondition.penaltyValue * boundaryCondition.value


    @requiresInitialized
    def applyNeumannBoundary( self, boundaryCondition ):
        self.load[boundaryCondition.DoF]    = boundaryCondition.value
        boundaryCondition.applied           = True


    @requiresInitialized
    def removeNeumannBoundary( self, boundaryCondition ):
        self.load[boundaryCondition.DoF]    -= boundaryCondition.value
        boundaryCondition.applied           = False


    @requiresInitialized
    def sample( self, solution, samples ):
        values = np.zeros( samples.shape )
        for element in self.elements:
            values += element( solution[element.DoFs], samples )

        return values


    @requiresInitialized
    def updateLoad( self, load ):
        '''
        Set load on all elements and reintegrate
        '''
        # Clear
        self.load = np.zeros( self.load.shape )
        
        # Set load on all elements and integrate
        for element in self.elements:
            element.load = load
            element.integrateLoad( self.load )

        # Reapply boundary conditions
        for BC in self.boundaries:
            self.applyBoundaryCondition( BC )




class TransientFEModel( FEModel ):
    '''
    FEModel extended with mass matrix and time-and-space-dependent load function
    '''
    def __init__( self, *args, loadFunction=None, **kwargs ):
        FEModel.__init__( self, *args, **kwargs )

        # Initialize
        self.mass           = None
        self._loadFunction  = loadFunction
        self.time           = 0.0


    def resetMatrices( self ):
        FEModel.resetMatrices(self)
        self.mass.data      = np.zeros( self.mass.data.shape, dtype=self.mass.data.dtype )


    @property
    def loadFunction( self ):
        if self._loadFunction is None:
            raise AttributeError( "Unset load function!" )

        return self._loadFunction


    def allocateZeros( self ):
        DoFs = FEModel.allocateZeros( self )

        if self.mass is not None:
            raise RuntimeError( "Attempt to initialize existing matrices" )
        self.mass   = sparse.csr_matrix((   np.zeros( DoFs.shape[1] ), DoFs ), 
                                            shape=self.shape )
        return DoFs


    @requiresInitialized
    def integrate( self, *args, **kwargs ):
        FEModel.integrate( self, *args, **kwargs )
        
        for element in self.elements:
            element.integrateMass( self.mass, *args, **kwargs )


    @requiresInitialized
    def updateTime( self, time ):
        self.time = time
        self.updateLoad( lambda x: self.loadFunction( time, x ) )
        for boundaryCondition in self.boundaries:
            self.applyBoundaryCondition( boundaryCondition )


    @requiresInitialized
    def addBoundaryCondition( self, boundaryCondition ):
        if callable(boundaryCondition.value) is False:
            raise AttributeError( "Boundary condition value for a TransientFEModel must be a function of time!" ) 
        return FEModel.addBoundaryCondition( self, boundaryCondition )


    @requiresInitialized
    def applyBoundaryCondition( self, boundaryCondition ):
        BC          = copy(boundaryCondition)
        BC.value    = BC.value( self.time )
        return FEModel.applyBoundaryCondition( self, BC )


    @requiresInitialized
    def removeBoundaryCondition( self, boundaryCondition ):
        BC          = copy(boundaryCondition)
        BC.value    = BC.value( self.time )
        return FEModel.removeBoundaryCondition( self, BC )




class NonlinearFEModel( FEModel ):
    '''
    FEModel with material nonlinearities
    '''
    def __init__( self, *args, **kwargs ):
        FEModel.__init__( self, *args, **kwargs )
        self.geometricStiffness = None

    
    def resetMatrices( self ):
        FEModel.resetMatrices(self)
        self.geometricStiffness.data = np.zeros(    self.geometricStiffness.data.shape,
                                                    self.geometricStiffness.data.dtype )


    def allocateZeros( self ):
        DoFs                    = FEModel.allocateZeros( self )
        self.geometricStiffness = self.stiffness.copy()
        return DoFs


    @requiresInitialized
    def integrate( self, *args, **kwargs ):
        for element in self.elements:
            element.integrateStiffness( self.stiffness, *args, **kwargs )
            element.integrateLoad( self.load, *args, **kwargs )
            element.integrateGeometricStiffness( self.geometricStiffness, *args, **kwargs )


    #@requiresInitialized
    #def applyNeumannBoundary( self, boundaryCondition, fieldValue ):
    #    # TODO
    #    FEModel.applyNeumannBoundary( self, boundaryCondition )


