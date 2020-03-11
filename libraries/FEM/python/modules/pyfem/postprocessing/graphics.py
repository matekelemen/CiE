# --- Python Imports ---
from matplotlib import pyplot as plt
from matplotlib import animation as anim
import numpy as np

# --- Internal Imports ---


# ---------------------------------------------------------
def animateTimeSeries( time, positions, timeSeriesSolution, model, speed=0.1, repeat=True, ylim=(-1.0,1.0), figure=None, axis=None ):
    # Pack into list in only one time series is supplied
    if len( timeSeriesSolution.shape ) < 3:
        timeSeriesSolution = np.asarray([timeSeriesSolution])

    values  = [model.sample( timeSeries[0], positions ) for timeSeries in timeSeriesSolution]
    blit    = True

    # Create figure and axis if not given
    if figure is None or axis is None:
        figure, axis    = plt.subplots()

    # Initialize lines
    lines       = [ None for i in range(len(timeSeriesSolution)) ]
    for index, valueSet in enumerate( values ):
        lines[index], = axis.plot( positions, valueSet )
    timeLabel   = axis.text(    0.01,  
                                0.95, 
                                "t = %.3f" % time[0],
                                fontdict={  "family"    : "serif",
                                            "alpha"     : 0.2,
                                            "weight"    : "bold"} )
    axis.set_ylim(ylim)

    # Set callbacks
    def frameGenerator():
        for index, t in enumerate(time):
            yield ( t, [model.sample( timeSeries[index], positions ) for timeSeries in timeSeriesSolution] )

    def initAnim():
        for line in lines:
            line.set_ydata( [np.nan] * len(positions) )
        timeLabel.set_text( "t = %.3f" % time[0] )
        return (*lines, timeLabel)

    def stepAnim( args ):
        # Update time
        timeLabel.set_text( "t = %.3f" % args[0] )

        # Update plot
        for index, line in enumerate( lines ):
            line.set_ydata( args[1][index] )
        return (*lines, timeLabel)

    # Create animation
    animation = anim.FuncAnimation( figure,
                                    stepAnim,
                                    init_func=initAnim,
                                    frames=frameGenerator,
                                    interval=1.0/speed,
                                    blit=blit,
                                    save_count=50,
                                    repeat_delay=1000,
                                    repeat=repeat)
    return animation