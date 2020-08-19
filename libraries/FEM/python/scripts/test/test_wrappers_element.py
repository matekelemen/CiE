# --- Internal Imports ---
import pyfem.pyfem as fem

# --- Python Imports ---
import unittest
import numpy as np


class StaticLinearHeatElement1DLinear(unittest.TestCase):

    def test_integrateStiffness( self ):
        ''' '''
        stiffness = [
            [ 0.0, 0.0, 0.0 ],
            [ 0.0, 0.0, 0.0 ],
            [ 0.0, 0.0, 0.0 ]
        ]

        # Define updater
        def updateStiffness( i, j, value ):
            stiffness[i][j] += value

        # Instantiate an element
        materialParameter   = 2.0
        domain              = [ 0.0, 1.0 ]
        DoFs                = [ 0, 2 ]
        element             = fem.StaticLinearHeatElement1DLinear(  materialParameter,
                                                                    domain,
                                                                    DoFs )

        # Test integration
        element.integrateStiffness(updateStiffness)
        for i,row in enumerate(stiffness):
            for j,value in enumerate(row):
                if not (i in DoFs) or not (j in DoFs):
                    self.assertEqual( value, 0.0 )
                elif i==j:
                    self.assertEqual( value, materialParameter )
                else:
                    self.assertEqual( value, -materialParameter )

    
    def test_integrateStiffness_list( self ):
        ''' '''
        numberOfElements    = 150
        stiffness           = np.zeros( (numberOfElements+1,numberOfElements+1) )
        elements            = fem.StaticLinearHeatElement1DLinear_List()
        materialParameter   = 2.0
        domains             = [ [float(i),float(i)+1.0] for i in range(numberOfElements) ]
        DoFs                = [ [i,i+1] for i in range(numberOfElements) ]
        for elementIndex in range(numberOfElements):
            elements.emplace_back(  materialParameter,
                                    domains[elementIndex],
                                    DoFs[elementIndex] )

        def updateStiffness( i, j, value ):
            stiffness[i][j] += value
        fem.integrateStiffness( elements, updateStiffness )
        
        for i,row in enumerate(stiffness):
            for j,value in enumerate(row):
                if i == j:                                  # <-- main diagonal
                    if i!=DoFs[0][0] and i!=DoFs[-1][-1]:       # <-- internal DoF
                        self.assertEqual( value, 2.0*materialParameter )
                    else:                                       # <-- boundary DoF
                        self.assertEqual( value, materialParameter )
                elif abs(i-j) == 1:                         # <-- first band
                    self.assertEqual( value, -materialParameter )
                else:                                       # <-- non-DoF
                    self.assertEqual( value, 0.0 )
        


if __name__ == "__main__":
    unittest.main()