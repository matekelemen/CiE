# --- Python Imports ---
import numpy as np
from numpy.polynomial.legendre import Legendre, legint

# --- Utility Imports ---
from pyfem.utilities import *

# ---------------------------------------------------------
class BasisFunctions:
    '''
    Abstract base class for a matching collection of 1D basis functions

    Constructor arguments:
        functions=None          : basis functions to store
        dtype=np.float64        : storage and function return data types
    '''
    def __init__( self, functions=None, dtype=np.float64 ):
        self.dtype      = dtype
        self.domain     = np.array( (-1.0, 1.0), dtype=dtype )
        self._functions = functions

    

    def evaluate( self, basisID, position ):
        # Check if the function was set
        if self._functions is None:
            raise AttributeError("Unset basis function!")

        # Evaluate at a single point: check bounds and evaluate
        if not isNumpyArray(position):
            if position<self.domain[0] or position>self.domain[1]:
                raise ValueError( "Argument for basis function is out of bounds!" )
            return self.dtype( self._functions[basisID](position) )
        
        # Evaluate at multiple points: check bounds and evaluate
        else:
            if np.min( position )<self.domain[0] or np.max(position)>self.domain[1]:
                raise ValueError( "Argument for basis function is out of bounds!" )
            return np.asarray( [self._functions[basisID](x) for x in position], dtype=self.dtype )



    def evaluateAll( self, position ):
        if isArray( position ):
            return np.asarray( [self.evaluate(basisID,position) for basisID in range(len(self._functions))] )
        else:
            raise ValueError( "Expecting an array argument!" )


    def __call__( self, basisID, position ):
        return self.evaluate( basisID, position )



class PolynomialBasisFunctions( BasisFunctions ):
    '''
    1D polynomial basis function abstract base class

    Constructor arguments:
        polynomialOrder=1       : polynomial order of the basis to compute the number of functions
        functions=None          : basis functions to store
        dtype=np.float64        : storage and function return data types
    '''
    def __init__( self, *args, polynomialOrder=1, **kwargs ):
        BasisFunctions.__init__( self, *args, **kwargs )
        self.polynomialOrder = polynomialOrder



class LinearBasisFunctions( PolynomialBasisFunctions ):
    '''
    Classic linear basis functions

    Constructor arguments:
        functions=None          : basis functions to store
        dtype=np.float64        : storage and function return data types
    '''
    def __init__( self, *args, **kwargs ):
        PolynomialBasisFunctions.__init__( self, *args, **kwargs )
        self._functions = [ lambda t: 0.5 - t/2.0,
                            lambda t: 0.5 + t/2.0 ]


    def derivatives( self ):
        return BasisFunctions(  functions = [   lambda t: -0.5,
                                                lambda t: 0.5 ] )



class HierarchicBasisFunctions( PolynomialBasisFunctions ):
    '''
    Legendre polynomials with the constant function replaced by a linear one
    (using the numpy implementation of Legendre polynomials)

    Constructor arguments:
        polynomialOrder=1       : polynomial order of the basis to compute the number of functions
        dtype=np.float64        : storage and function return data types
    '''
    def __init__( self, *args, **kwargs ):
        PolynomialBasisFunctions.__init__( self, *args, **kwargs )

        # Standard linear basis functions if the polynomial order is 1
        if self.polynomialOrder is 1:
            self._functions = [ Legendre( (0,-1) ),
                                Legendre( (0,1) ) ]
        
        # Legendre polynomials, but replace the constant function with a linear one
        elif self.polynomialOrder > 1:
            self._functions = [None for i in range(self.polynomialOrder+1)]
            for index, coefficients in enumerate( oneHotArray(self.polynomialOrder+1) ):

                if index is 0:
                    coefficients[0] = 0
                    coefficients[1] = -1

                self._functions[index] = Legendre(  coefficients,
                                                    domain=self.domain  )
        
        else:
            raise ValueError( "Invalid polynomial order!" )


    def derivatives( self ):
        functions = [ Legendre(np.polynomial.legendre.legder(polynomial.coef)) for polynomial in self._functions ]
        return BasisFunctions( functions=functions )




class IntegratedHierarchicBasisFunctions( PolynomialBasisFunctions ):
    '''
    Hierarchic basis functions as proposed by Szabó & Babuska

    Constructor arguments:
        polynomialOrder=1       : polynomial order of the basis to compute the number of functions
        dtype=np.float64        : storage and function return data types
    '''
    def __init__( self, *args, **kwargs ):
        PolynomialBasisFunctions.__init__( self, *args, **kwargs )

        # Standard linear basis functions if the polynomial order is 1
        if self.polynomialOrder is 1:
            self._functions = [ Legendre( (0,-1) ),
                                Legendre( (0,1) ) ]
        
        # Integrated Legendre polynomials (first two functions replaced by classic linear ones)
        elif self.polynomialOrder > 1:
            # Base
            self._functions     = [ Legendre( coefficients ) for coefficients in oneHotArray(self.polynomialOrder+1) ]

            # Replace the constant function
            self._functions[0]  = Legendre( (0,-1) )

            # Integrate base functions
            for index in range( self.polynomialOrder, 1, -1 ):
                self._functions[index] = Legendre( legint( np.sqrt(float(index) - 0.5) * self._functions[index-1].coef,
                                                                    m=1,
                                                                    lbnd=-1.0),
                                                            domain=self.domain  )
        
        else:
            raise ValueError( "Invalid polynomial order!" )


    def derivatives( self ):
        functions = [ Legendre(np.polynomial.legendre.legder(polynomial.coef)) for polynomial in self._functions ]
        return BasisFunctions( functions=functions )

