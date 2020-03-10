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
        for name, value in vars(instance).items():
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
        self.stiffness  = sparse.csc_matrix(    ( np.zeros( DoFs.shape[1] ), DoFs ), 
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


    def addBoundaryCondition( self, boundaryCondition ):
        self.boundaries.append( boundaryCondition )
        self.applyBoundaryCondition( boundaryCondition )
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
    def applyBoundaryCondition( self, boundaryCondition ):
        if boundaryCondition.BCType is "dirichlet":
            self.applyDirichletBoundary( boundaryCondition )
        elif boundaryCondition.BCType is "neumann":
            self.applyNeumannBoundary( boundaryCondition )
        else:
            raise NotImplementedError( "Unknown boundary condition type" )


    @requiresInitialized
    def applyDirichletBoundary( self, boundaryCondition ):
        if boundaryCondition.applied is not True:
            self.stiffness[boundaryCondition.DoF,boundaryCondition.DoF] += boundaryCondition.penaltyValue
            boundaryCondition.applied = True
        self.load[boundaryCondition.DoF]    += boundaryCondition.penaltyValue * boundaryCondition.value


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
    def solveStationary( self ):
        '''
        Solve current system.
        '''
        x, info = sparse.linalg.gmres(  self.stiffness, 
                                        self.load,
                                        x0=np.random.rand(self.size),
                                        atol=1e-12,
                                        maxiter=np.max((5*self.size,100)) )
        if info<0:
            raise RuntimeError( "Error solving the system" )
        elif info>0:
            print( "Solution has not converged after " +str(info) + " iterations" )
        
        return x


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


    @property
    def loadFunction( self ):
        if self._loadFunction is None:
            raise AttributeError( "Unset load function!" )

        return self._loadFunction


    def allocateZeros( self ):
        DoFs = FEModel.allocateZeros( self )

        if self.mass is not None:
            raise RuntimeError( "Attempt to initialize existing matrices" )
        self.mass   = sparse.csc_matrix((   np.zeros( DoFs.shape[1] ), DoFs ), 
                                            shape=self.shape )
        return DoFs


    @requiresInitialized
    def integrate( self ):
        FEModel.integrate( self )
        
        for element in self.elements:
            element.integrateMass( self.mass )


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