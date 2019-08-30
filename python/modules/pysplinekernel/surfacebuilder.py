from pysplinekernel.surfacekernel import SurfaceKernel

class SurfaceGUI:
    # CONSTRUCTOR -------------------------------------------------------------
    def __init__(self):
        self.fig    = plt.figure()
        self.ax     = self.fig.add_subplot(111)
        self.kernel = SplineKernel()
        self.title  = "Spline GUI"
        self.reset()
    # GARBAGE COLLECTION AND RESET --------------------------------------------
    def reset(self):
        # Data
        self.kernel.reset()
        self.cursorPoint = []
        # State
        self.enableMotion   = True
        self.enableClick    = True
        self.enableKeyPress  = True
        # Figure
        self.ax.set_title(self.title, loc='left', fontsize=14)
        self.fig.tight_layout()
        # Axes
        self.ax.set_xlim( left=0, right=1, auto=False )
        self.ax.set_ylim( bottom=0, top=1, auto=False )
        # Reset objects
        self.artists = {
        "spline"        : self.ax.plot(2,2,'b-',linewidth=1),
        "polygon"       : self.ax.plot(2,2,'r-.',linewidth=0.5),
        "cursor"        : self.ax.plot(2,2,'b+'),
        "points"        : self.ax.plot(2,2,'bo')
        }
        self.enableArtists = {
        "spline"        : True,
        "polygon"       : False,
        "cursor"        : True,
        "points"        : True    
        }
        
    # DRAW FUNCTIONS ----------------------------------------------------------
    def draw(self, targets=None, data=None):
        # Hide object if called with no data argument
        if targets!=None and data!=None:
            for target,dat in zip(targets,data):
                if self.enableArtists[target]:
                    self.artists[target][0].set_xdata(dat[0])
                    self.artists[target][0].set_ydata(dat[1])
                else:
                    self.artists[target][0].set_xdata(2)
                    self.artists[target][0].set_ydata(2)
        # Clean background
        self.ax.draw_artist(self.ax.patch)
        # Draw every enabled artist
        for key,artist in self.artists.items():
            if self.enableArtists[key]:
                self.ax.draw_artist(artist[0])
        # Update canvas
        self.fig.canvas.update()
        #self.fig.canvas.flush_events()
        
    def show(self,target,forceDraw=False):
        self.enableArtists[target] = True
        if forceDraw:
            self.draw()
        
    def hide(self,target,forceDraw=False):
        self.enableArtists[target] = False
        if forceDraw:
            self.draw()
            
    def switch(self,target,forceDraw=False):
        if self.enableArtists[target]:
            self.hide(target,forceDraw)
        else:
            self.show(target,forceDraw)
        
    # EVENT HANDLERS ----------------------------------------------------------
    def onMotion(self,event,bypass=False):
        if self.enableMotion or bypass:
            if event.inaxes != None:
                # Re-enable cursor if needed
                if self.enableArtists['cursor'] == False:
                    self.show('cursor',True)
                # Gather cursor data
                self.cursorPoint = [event.xdata,event.ydata]
                # Gather spline data
                self.kernel.getPoints(self.cursorPoint)
                # Draw
                self.draw(['cursor','spline','polygon'],[self.cursorPoint,self.kernel.curvePoints, self.kernel.controlPoints])
            else:
                self.hide('cursor',True)
                
    def onClick(self,event,bypass=False):
        if self.enableClick or bypass:
            if event.inaxes != None:
                # Record interpolation point
                self.kernel.push([event.xdata,event.ydata])
                # Draw interpolation points
                self.draw(['points'],[self.kernel.interpolationPoints])
        
    def onKeyPress(self,event,bypass=False):
        if self.enableKeyPress or bypass:
            # ESCAPE
            if event.key == 'escape':
                self.reset()
            # ENTER
            elif event.key == 'enter':
                self.switch('polygon',False)
                self.draw(['polygon'],[self.kernel.controlPoints])
            # SPACE
            elif event.key == ' ':
                self.enableMotion = not self.enableMotion
                self.enableClick = not self.enableClick
            # NUMBERS
            elif 49 <= ord(event.key) and ord(event.key) <= 57:
                self.kernel.polynomialOrder = int(event.key)
                self.kernel.getPoints(self.cursorPoint)
                self.draw(['cursor','spline','polygon'],[self.cursorPoint,self.kernel.curvePoints, self.kernel.controlPoints])