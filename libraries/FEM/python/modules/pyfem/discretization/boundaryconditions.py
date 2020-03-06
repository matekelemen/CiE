
# ---------------------------------------------------------
class BoundaryCondition:
    '''
    Boundary condition base class
    '''
    def __init__( self, DoF, value, penaltyValue=1e5 ):
        self.DoF            = DoF
        self.value          = value
        self.BCType         = None
        self.penaltyValue   = penaltyValue


class DirichletBoundary( BoundaryCondition ):
    def __init__( self, *args, **kwargs ):
        BoundaryCondition.__init__( self, *args, **kwargs )
        self.BCType = "dirichlet"


class NeumannBoundary( BoundaryCondition ):
    def __init__( self, *args, **kwargs ):
        BoundaryCondition.__init__( self, *args, **kwargs )
        self.BCType = "neumann"