# --- Python Imports ---
import numpy as np
import scipy.sparse as sparse
import functools

# --- Internal Imports ---
from pyfem.utilities import isNumpyArray, gridPairs
from .element import Element, Element1D

# ---------------------------------------------------------
# Decorators
def requiresInitialized( function ):
    @functools.wraps( function )
    def decoratedFunction( *args, **kwargs ):
        if args[0].stiffness is None or args[0].mass is None or args[0].load is None or args[0].elements is None:
            raise RuntimeError( "Instance needs to be initialized first" )
        else:
            return function( *args, **kwargs )
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
        self.mass           = None
        self.load           = None


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
        if self.stiffness is not None or self.mass is not None or self.load is not None:
            raise RuntimeError( "Attempt to initialize existing matrices" )

        # Initialize matrices
        self.stiffness  = sparse.csc_matrix(    ( np.zeros( DoFs.shape[1] ), DoFs ), 
                                                shape=self.shape )
        self.mass       = sparse.csc_matrix(    ( np.zeros( DoFs.shape[1] ), DoFs ), 
                                                shape=self.shape )
        self.load       = np.zeros( (self.size, 1) )


    @requiresInitialized
    def integrate( self ):
        '''
        Loop through elements and integrate their matrices
        '''
        for element in self.elements:
            element.integrateStiffness( self.stiffness )
            element.integrateMass( self.mass )
            element.integrateLoad( self.load )


    @requiresInitialized
    def applyDirichletBoundary( self, DoF, value, penaltyValue=1e8 ):
        self.stiffness[DoF,DoF] += penaltyValue
        self.load[DoF]          += penaltyValue * value


    @requiresInitialized
    def applyNeumannBoundary( self, DoF, value ):
        self.load[DoF]          = value