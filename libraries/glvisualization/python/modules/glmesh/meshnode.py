# --- Python imports ---
import numpy as np
from warnings import warn

# --- Vispy imports ---
from vispy.geometry import MeshData

# --- Internal imports ---
from glmesh import checkID

# -----------------------------------------------------
class MeshNode(MeshData):
    def __init__(self, parent=None, children=None, objectID=0, materialID=0, **kwargs):
        MeshData.__init__(self,**kwargs)

        # Set parent (None=root)
        self._parent    = parent

        # Set children
        if children is not None:
            if isinstance(children,list):
                self._children  = children
            else:
                self._children  = [children]
        else:
            self._children  = []

        # Set IDs
        self._objectID      = objectID
        self._materialID    = materialID

        checkID(self,self._objectID)
        checkID(self,self._materialID)

        # Set empty array for unset data, ndarray for set data
        if self._vertices is None:
            self._vertices          = np.empty(0, dtype=np.float32)
        elif isinstance( self._vertices, list ) or isinstance( self._vertices, tuple ):
            self._vertices          = np.asarray( self._vertices, dtype=np.float32 )
        elif not isinstance( self._vertices, np.ndarray ):
            raise ValueError('Invalid container type!')            

        if self._faces is None:
            self._faces             = np.empty(0, dtype=np.uint32)
        elif isinstance( self._faces, list ) or isinstance( self._faces, tuple ):
            self._faces             = np.asarray( self._faces, dtype=np.uint32 )
        elif not isinstance( self._faces, np.ndarray ):
            raise ValueError('Invalid container type!')     

        if self._vertex_colors is None:
            self._vertex_colors     = np.empty(0, dtype=np.float32)
        elif isinstance( self._vertex_colors, list ) or isinstance( self._vertex_colors, tuple ):
            self._vertex_colors     = np.asarray( self._vertex_colors, dtype=np.float32 )
        elif not isinstance( self._vertex_colors, np.ndarray ):
            raise ValueError('Invalid container type!')  


    def parent(self):
        return self._parent


    def setParent(self, parent):
        self._parent = parent


    def child(self,index):
        return self._children[index]


    def children(self):
        return self._children

    
    def addChild(self,child):
        self._children.append(child)
        child.setParent(self)


    def computeCompiledMeshSize(self):
        '''
        Recursively compute the number of vertices in the entire mesh tree
        '''
        # Prealloc
        sizes   = { 'vertices'      : 0,
                    'vertex_colors' : 0,
                    'nodes'         : 0}

        # Accumulate children sizes
        if self._children:
            for child in self._children:
                childSizes  = child.computeCompiledMeshSize()
                for name, value in childSizes.items():
                    sizes[name] += value
                    
        # Add self sizes
        sizes['vertices']       += len(self._faces)*3
        sizes['vertex_colors']  += len(self._vertex_colors)
        sizes['nodes']          += 1
            
        return sizes


    def getCompiledMesh(self, destinationDict=None, vertexIndex=0, vertexColorIndex=0):
        '''
        Get all vertices and triangles in the mesh
        '''
        # Create destination
        returnFlag  = False
        if destinationDict is None:
            returnFlag      = True
            sizes           = self.computeCompiledMeshSize()
            destinationDict = { 'vertices'          : np.empty( (sizes['vertices'],3),    dtype=np.float32),
                                'vertex_normals'    : np.empty( (sizes['vertices'],3),      dtype=np.uint32),
                                'vertex_colors'     : np.empty( (sizes['vertex_colors'],4), dtype=np.float32),
                                'objectIDs'         : np.empty( (sizes['vertices'],1),      dtype=np.uint32 ),
                                'materialIDs'       : np.empty( (sizes['vertices'],1),      dtype=np.uint32 )}

        # Write to container - vertices, normals, IDs
        for i, (vertices, normals) in enumerate( zip( self.get_vertices(indexed='faces'), self.get_vertex_normals(indexed='faces') ) ):
            for j, (vertex, normal) in enumerate( zip(vertices,normals) ):
                index   = vertexIndex + 3*i + j
                # Write vertices
                destinationDict['vertices'][index]                  = vertex
                # Write normals
                destinationDict['vertex_normals'][index]            = normal
                # Write IDs
                destinationDict['objectIDs'][index]                 = self._objectID
                destinationDict['materialIDs'][index]               = self._materialID
                
        # Write to container - colors
        if destinationDict['vertex_colors']:
            for i, triangle in enumerate(self.get_vertex_colors(indexed='faces')):
                for j, color in enumerate(triangle):
                    destinationDict['vertex_colors'][ vertexColorIndex + 3*i + j]   = color


        # Recursive call on the children
        for child in self._children:
            child.getCompiledMesh(  destinationDict=destinationDict,
                                    vertexIndex=vertexIndex+len(self._faces)*3,
                                    vertexColorIndex=len(self._vertex_colors) )

        # Return container if requested
        if returnFlag:
            return destinationDict



# -----------------------------------------------------
class TexturedMeshNode(MeshNode):
    def __init__(self,texture,**kwargs):
        MeshNode.__init__(self,**kwargs)
        # Set texture
        if isinstance( texture, np.ndarray ):
            pass