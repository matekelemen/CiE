import numpy as np
from vispy import scene
from pysplinekernel.editmarkervisual import EditMarkerVisual
from pysplinekernel.splinevisual import SplineVisual

class Canvas(scene.SceneCanvas):
    def __init__(self):
        # Initialize canvas
        scene.SceneCanvas.__init__(self, keys='interactive',
                                   size=(800, 600))
        # Create some initial points
        self.unfreeze()
        self.pos = np.empty((1,3),dtype=np.float32)
        # create new editable line
        self.visuals = {
            'markers'   : EditMarkerVisual(pos=self.pos, face_color='w', scaling=True),
            'spline'    : SplineVisual(pos=self.pos, color='w', width=1, antialias=False, method='gl')
            }
        # Add view and camera
        self.view = self.central_widget.add_view()
        self.view.camera = scene.PanZoomCamera(rect=(-100, -100, 200, 200),
                                               aspect=1.0)
        # the left mouse button pan has to be disabled in the camera, as it
        # interferes with dragging line points
        # Proposed change in camera: make mouse buttons configurable
        self.view.camera._viewbox.events.mouse_move.disconnect(
            self.view.camera.viewbox_mouse_event)
        # Visual objects
        self.view.add( self.visuals['markers'] )
        self.view.add( self.visuals['spline'] )
        scene.visuals.GridLines(parent=self.view.scene)
        # Add first point (cursor)
        self.cursor = True
        self.visuals['spline'].addPoint([0.0,0.0])
        # Initialization
        self.freeze()
        

    # MISC FUNCTIONS --------------------------------------------------------------------------------------

    # EVENT HANDLERS --------------------------------------------------------------------------------------
    def on_mouse_press(self, event):
        # Get mouse position
        tr  = self.scene.node_transform(self.visuals['markers'])
        pos = tr.map(event.pos)
        # Handle polygon
        self.visuals['markers'].selected_point, self.visuals['markers'].selected_index = self.visuals['markers'].selectPoint(pos)        
        if self.visuals['markers'].selected_point is None:
            # Append polygon
            self.visuals['markers'].addPoint(pos)
            # Set spline
            self.visuals['spline'].setPoint( pos[:2] )
            # Add new cursor point
            self.visuals['spline'].addPoint( [pos[0]+1e-4, pos[1]-1e-4] )
        else:
            # Delete cursor point
            self.cursor = False
            self.visuals['spline'].pop()
            

    def on_mouse_drag(self,pos):
        if self.visuals['markers'].selected_point is not None:
            # Snap to grid
            self.visuals['markers'].selected_point[0] = round(pos[0] / self.visuals['markers'].gridsize) * self.visuals['markers'].gridsize
            self.visuals['markers'].selected_point[1] = round(pos[1] / self.visuals['markers'].gridsize) * self.visuals['markers'].gridsize
            # Update polygon
            self.visuals['markers'].setPoint(self.visuals['markers'].selected_index,self.visuals['markers'].selected_point)
            self.visuals['markers'].updateMarkers(self.visuals['markers'].selected_index)
            # Update spline
            self.visuals['spline'].setPoint(self.visuals['markers'].selected_point,index=self.visuals['markers'].selected_index)

    def on_mouse_release(self,event):
        # Handle polygon
        self.visuals['markers'].selected_point = None
        self.visuals['markers'].selected_index = -1
        self.visuals['markers'].updateMarkers()
        # Add cursor point
        if not self.cursor:
            self.cursor = True
            self.visuals['spline'].addPoint([0.0,0.0])

    def on_mouse_move(self, event):
        # Position
        tr = self.scene.node_transform(self.visuals['markers'])
        pos = tr.map(event.pos)
        if event.button == 1:   # Drag
            self.on_mouse_drag(pos)
        else:                   # Move
            self.visuals['markers'].highlightMarkers(pos)
            self.visuals['spline'].setPoint(pos)
        
