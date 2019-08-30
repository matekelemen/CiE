import numpy as np
from vispy import app, scene

class EditLineVisual(scene.visuals.Line):
    # CONSTRUCTOR AND INITIALZER ----------------------------------------------------------------------
    def __init__(self, *args, **kwargs):
        scene.visuals.Line.__init__(self, *args, **kwargs)
        self.reset()
        
    def reset(self):
        self.unfreeze()
        # initialize point markers
        self.markers = scene.visuals.Markers(parent=self)
        self.marker_colors = np.ones((len(self.pos), 4), dtype=np.float32)
        self.markers.set_data(pos=self.pos, symbol="s", edge_color="red",
                              size=6)
        self.selected_point = None
        self.selected_index = -1
        # snap grid size
        self.gridsize = 5
        self.freeze()

    # MISC FUNCTIONS ------------------------------------------------------------
    def highlight_markers(self, pos_scene):
        #  if no button is pressed, just highlight the marker that would be
        # selected on click
        hl_point, hl_index = self.select_point(pos_scene)
        self.update_markers(hl_index, highlight_color=(0.5, 0.5, 1.0, 1.0))
        self.update()
        
    def update_markers(self, selected_index=-1, highlight_color=(1, 0, 0, 1)):
        """ update marker colors, and highlight a marker with a given color """
        self.marker_colors.fill(1)
        # default shape (non-highlighted)
        shape = "o"
        size = 6
        if 0 <= selected_index < len(self.marker_colors):
            self.marker_colors[selected_index] = highlight_color
            shape = "s"
            size = 8
        self.markers.set_data(pos=self.pos, symbol=shape, edge_color='red',
                              size=size, face_color=self.marker_colors)

    def select_point(self, pos_scene, radius=3):
        for index,p in enumerate(self.pos):
            if np.linalg.norm(pos_scene[:3] - p) < radius:
                return p, index
        # no point found, return None
        return None, -1

    def enable(self):
        self._visible = True
        self.draw()

    def disable(self):
        self._visible = False
        self.draw()

    # EVENT HANDLERS ------------------------------------------------------------
    def on_draw(self, event):
        # draw line and markers
        scene.visuals.Line.draw(self)
        self.markers.draw()

        # update markers and highlights
        self.update_markers(self.selected_index)


    def on_mouse_move(self,pos_scene):
        pass
