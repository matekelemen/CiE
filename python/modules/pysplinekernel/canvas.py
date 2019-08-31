import numpy as np
from vispy import app, scene
from pysplinekernel.editlinevisual import EditLineVisual
from pysplinekernel.splinevisual import SplineVisual

class Canvas(scene.SceneCanvas):
    def __init__(self):
        # Initialize canvas
        scene.SceneCanvas.__init__(self, keys='interactive',
                                   size=(1024, 768))
        # Create some initial points
        self.unfreeze()
        self.pos = np.empty((1,3),dtype=np.float32)
        # create new editable line
        self.visuals = {
            'polygon'   : EditLineVisual(pos=self.pos, color='r', width=0.5, antialias=True, method='gl'),
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
        self.view.add( self.visuals['polygon'] )
        self.view.add( self.visuals['spline'] )
        scene.visuals.GridLines(parent=self.view.scene)
        # Add first point
        self.visuals['spline'].addPoint([0.0,0.0])
        # Initialization
        self.show()
        self.freeze()

    # MISC FUNCTIONS --------------------------------------------------------------------------------------

    # EVENT HANDLERS --------------------------------------------------------------------------------------
    def on_mouse_press(self, event):
        # Get mouse position
        tr  = self.scene.node_transform(self.visuals['polygon'])
        pos = tr.map(event.pos)
        # Handle polygon
        self.visuals['polygon'].selected_point, self.visuals['polygon'].selected_index = self.visuals['polygon'].selectPoint(pos)        
        if self.visuals['polygon'].selected_point is None:
            # Append polygon
            self.visuals['polygon'].addPoint(pos)
            # Set spline
            self.visuals['spline'].setPoint( pos[:2] )
            # Add new cursor point
            self.visuals['spline'].addPoint( [pos[0]+1e-4, pos[1]-1e-4] )
            self.visuals['spline'].draw()
            

    def on_mouse_drag(self,pos):
        if self.visuals['polygon'].selected_point is not None:
            # Snap to grid
            self.visuals['polygon'].selected_point[0] = round(pos[0] / self.visuals['polygon'].gridsize) * self.visuals['polygon'].gridsize
            self.visuals['polygon'].selected_point[1] = round(pos[1] / self.visuals['polygon'].gridsize) * self.visuals['polygon'].gridsize
            # Update polygon
            self.visuals['polygon'].setPoint(self.visuals['polygon'].selected_index,self.visuals['polygon'].selected_point)
            self.visuals['polygon'].updateMarkers(self.visuals['polygon'].selected_index)
            # Update spline
            self.visuals['spline'].setPoint(self.visuals['polygon'].selected_point,index=self.visuals['polygon'].selected_index)

    def on_mouse_release(self,event):
        # Handle polygon
        self.visuals['polygon'].selected_point = None
        self.visuals['polygon'].selected_index = -1
        self.visuals['polygon'].updateMarkers()

    def on_mouse_move(self, event):
        # Position
        tr = self.scene.node_transform(self.visuals['polygon'])
        pos = tr.map(event.pos)
        # Drag
        if event.button == 1:
            self.on_mouse_drag(pos)
            self.visuals['spline'].draw()
        # Move
        else:
            self.visuals['polygon'].highlightMarkers(pos)
            self.visuals['spline'].setPoint(pos)
