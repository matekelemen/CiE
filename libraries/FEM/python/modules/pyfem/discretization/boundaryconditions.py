
# ---------------------------------------------------------
class BoundaryCondition:
    '''
    Boundary condition base class
    '''
    def __init__( self, DoF, value ):
        self.DoF            = DoF
        self.value          = value
        
        self.BCType         = None
        self.applied        = False


class PenaltyBoundaryCondition( BoundaryCondition ):
    def __init__( self, *args, penaltyValue=1e5, **kwargs ):
        BoundaryCondition.__init__( self, *args, **kwargs )
        self.penaltyValue   = penaltyValue


class DirichletBoundary( PenaltyBoundaryCondition ):
    def __init__( self, *args, **kwargs ):
        PenaltyBoundaryCondition.__init__( self, *args, **kwargs )
        self.BCType = "dirichlet"


class NeumannBoundary( BoundaryCondition ):
    def __init__( self, *args, **kwargs ):
        BoundaryCondition.__init__( self, *args, **kwargs )
        self.BCType = "neumann"