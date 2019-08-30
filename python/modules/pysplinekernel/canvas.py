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
        # Add visuals
        self.view.add( self.visuals['polygon'] )
        self.view.add( self.visuals['spline'] )
        self.show()
        self.selected_point = None
        scene.visuals.GridLines(parent=self.view.scene)
        self.freeze()

    # MISC FUNCTIONS --------------------------------------------------------------------------------------

    # EVENT HANDLERS --------------------------------------------------------------------------------------
    def on_mouse_press(self, event):
        # Get mouse position
        tr  = self.scene.node_transform(self.visuals['polygon'])
        pos = tr.map(event.pos)
        # Handle polygon
        self.visuals['polygon'].selected_point, self.visuals['polygon'].selected_index = self.visuals['polygon'].select_point(pos)
        if self.selected_point is None:
            # Update selection
            self.visuals['polygon']._pos = np.append(self.visuals['polygon'].pos, [pos[:3]], axis=0)
            self.visuals['polygon'].set_data(pos=self.visuals['polygon'].pos)
            self.visuals['polygon'].marker_colors = np.ones((len(self.visuals['polygon'].pos), 4), dtype=np.float32)
            self.visuals['polygon'].selected_point = self.visuals['polygon'].pos[-1]
            self.visuals['polygon'].selected_index = len(self.visuals['polygon'].pos) - 1
        # Handle spline
        
        
        # Register point in splinekernel
        if self.visuals['polygon'].selected_index == len(self.visuals['polygon'].pos)-1:
            # Wait for mouse release
            self.on_mouse_move(event)
            self.visuals['spline'].addInterpolationPoint( pos[:2] )
            self.visuals['spline'].draw()

    def on_mouse_drag(self,pos):
        if self.visuals['polygon'].selected_point is not None:
            # update selected point to new position given by mouse
            self.visuals['polygon'].selected_point[0] = round(pos[0] / self.visuals['polygon'].gridsize) * self.visuals['polygon'].gridsize
            self.visuals['polygon'].selected_point[1] = round(pos[1] / self.visuals['polygon'].gridsize) * self.visuals['polygon'].gridsize
            self.visuals['polygon'].set_data(pos=self.visuals['polygon'].pos)
            self.visuals['polygon'].update_markers(self.visuals['polygon'].selected_index)

    def on_mouse_release(self,event):
        # Handle polygon
        self.visuals['polygon'].selected_point = None
        self.visuals['polygon'].update_markers()

    def on_mouse_move(self, event):
        # left mouse button
        tr = self.scene.node_transform(self.visuals['polygon'])
        pos = tr.map(event.pos)
        if event.button == 1:
            self.on_mouse_drag(pos)
            self.visuals['spline'].draw()
        else:
            self.visuals['polygon'].on_mouse_move(pos)
            self.visuals['polygon'].highlight_markers(pos)
            self.visuals['spline'].splineKernel.interpolationPoints[0][-1] = pos[0]
            self.visuals['spline'].splineKernel.interpolationPoints[1][-1] = pos[1]
