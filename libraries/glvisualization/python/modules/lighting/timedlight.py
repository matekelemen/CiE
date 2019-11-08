from lighting.simplelight import SimpleLight

class TimedSpotLight(SimpleLight):
    def __init__(self,parent,pathFunction=None,*args,**kwargs):
        SimpleLight.__init__(self,parent,**kwargs)

        if pathFunction is None:
            pathFunction = defaultPathFunction
        self._pathFunction = pathFunction

        self._t0    = default_timer()
        self._timer = app.Timer('auto', connect=self.on_timer, start='true')

    
    def on_timer(self,event):
        self.update(lightPos=self._pathFunction( default_timer()-self._t0 ))

    



def defaultPathFunction(t):
    return np.array([ np.cos(t), np.sin(t), 1.0 ],dtype=np.float32)