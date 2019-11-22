# --- Python imports ---
import numpy as np

# --- Vispy imports ---
from vispy import scene, app

# --- Visualization imports ---
from pysplinekernel import EditMarkerVisual, EditLineVisual

# --- CSG imports ---
import pycsg as csg

# -----------------------------------------------------------
class ModularCanvas(scene.SceneCanvas):

    def __init__(self):
        scene.SceneCanvas.__init__(self,    keys='interactive',
                                            size=(800, 600))

        self.unfreeze()

        # Initialize backend
        self.view           = self.central_widget.add_view()
        self.view.camera    = scene.PanZoomCamera(  rect=(-100, -100, 200, 200),
                                                    aspect=1.0)

        # Initialize state storage
        self.eventClass     = EventHandlerClass(canvas=self)
        self.objects        = {}
        self.observers      = []

        self.freeze()


    def addVisualObject(self,visualObjectName,visualObject):
        self.objects[visualObjectName]  = visualObject
        self.view.add( self.objects[visualObjectName] )


    def bindEvents(self,eventClass):
        self.eventClass     = eventClass
        eventClass.canvas   = self


    def attachObserver(self,observer):
        self.observers.append(observer)
        observer.attach(self)


    def detachObserver(self,observer):
        for index, peep in enumerate(self.observers):
            if peep is observer:
                peep.detach()
                del self.observers[index]


    def updateObservers(self, event):
        for observer in self.observers:
            observer.onSubjectChange(event)


    # EVENT HANDLER ROUTERS --------------------------------
    def on_mouse_press(self,event):
        self.eventClass.on_mouse_press(event)


    def on_mouse_release(self,event):
        self.eventClass.on_mouse_release(event)


    def on_mouse_move(self,event):
        self.eventClass.on_mouse_move(event)






class LineMarkerCanvas(ModularCanvas):

    def __init__(self):
        ModularCanvas.__init__(self)
        
        self.unfreeze()

        initPos     = np.empty((1,3),dtype=np.float32)
        self.addVisualObject('markers', EditMarkerVisual(   pos=initPos,
                                                            face_color='w',
                                                            scaling=True)   )
        self.addVisualObject('lines',   EditLineVisual(     pos=initPos     ))

        self.eventClass = LineMarkerCanvasCreateEvents(self)
        self.bindEvents(self.eventClass)

        self.freeze()


    def getPos(self,pos):
        transform   = self.scene.node_transform(self.objects['markers'])
        return transform.map(pos)


    def addPoint(self,point):
        self.objects['markers'].addPoint(point)
        self.objects['lines'].addPoint(point)
        self.updateObservers(None)


    def setPoint(self,index,point):
        self.objects['markers'].setPoint(index,point)
        self.objects['lines'].setPoint(index,point)
        self.updateObservers(None)





# EVENT HANDLER CLASSES ---------------------------------
class EventHandlerClass:
    def __init__(self,canvas=None):
        self.canvas = canvas

    def on_mouse_press(self,event):
        pass

    def on_mouse_release(self,event):
        pass

    def on_mouse_move(self,event):
        pass



class LineMarkerCanvasCreateEvents(EventHandlerClass):
    def __init__(self,*args,**kwargs):
        EventHandlerClass.__init__(self,*args,**kwargs)

    def on_mouse_press(self,event):
        if event.button==1:
            pos = self.canvas.getPos(event.pos)
            self.canvas.addPoint(pos)
        elif event.button==2:
            self.canvas.bindEvents( LineMarkerCanvasEditEvents() )

    def on_mouse_release(self,event):
        self.canvas.objects['markers'].updateMarkers()

    def on_mouse_move(self,event):
        pass


class LineMarkerCanvasEditEvents(EventHandlerClass):
    def __init__(self,*args,**kwargs):
        EventHandlerClass.__init__(self,*args,**kwargs)

    def on_mouse_press(self,event):
        if event.button==1:
            pos = self.canvas.getPos(event.pos)
            self.canvas.objects['markers'].selected_point, self.canvas.objects['markers'].selected_index = self.canvas.objects['markers'].selectPoint(pos)
        elif event.button==2:
            self.canvas.bindEvents( LineMarkerCanvasCreateEvents() )

    def on_mouse_release(self,event):
        # Clear selection
        self.canvas.objects['markers'].selected_point = None
        self.canvas.objects['markers'].selected_index = -1
        self.canvas.objects['markers'].updateMarkers()

    def on_mouse_move(self,event):
        if event.button == 1:   
            self.on_mouse_drag(event)

    def on_mouse_drag(self,event):
        pos = self.canvas.getPos(event.pos)
        if self.canvas.objects['markers'].selected_point is not None:
            # Snap to grid
            self.canvas.objects['markers'].selected_point = pos
            # Update polygon
            self.canvas.setPoint(self.canvas.objects['markers'].selected_index,self.canvas.objects['markers'].selected_point)
            self.canvas.objects['markers'].updateMarkers(self.canvas.objects['markers'].selected_index)








class Observer:
    def __init__(self,subject):
        self.subject = subject

    def attach(self,subject):
        self.subject = subject

    def detach(self):
        self.subject = None

    def onSubjectChange(self,event):
        pass




class MinimumDiscObserver(Observer):
    def __init__(self,subject):
        Observer.__init__(self,subject)

    def attach(self,subject):
        subject.addVisualObject('minimum_disc',EditLineVisual(pos=np.empty((50,3),dtype=np.float32)))


    def detach(self):
        del self.subject.objects['minimum_disc']
        self.subject=None

    def onSubjectChange(self,event):
        pts     = np.transpose( self.subject.objects['markers'].pos, (1,0) )[:2,:]
        disc    = csg.MinimumEnclosingDisc(np.transpose(pts,(1,0)))
        disc.build(1e-10)
        center  = disc.getCenter()
        radius  = disc.getRadius()

        n = self.subject.objects['minimum_disc'].pos.size/3
        for i in range(int(n)):
            phi = i*2*np.pi/(n-1)
            self.subject.objects['minimum_disc'].pos[i,:] = [
                center[0]+radius*np.cos(phi),
                center[1]+radius*np.sin(phi),
                0.0
            ]
        self.subject.objects['minimum_disc'].update()


canvas = LineMarkerCanvas()
canvas.attachObserver(MinimumDiscObserver(canvas))
canvas.objects['lines'].visible = False

canvas.view.camera._viewbox.events.mouse_move.disconnect(
            canvas.view.camera.viewbox_mouse_event)


canvas.show()
app.run()