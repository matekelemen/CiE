# --- Python Imports ---
from matplotlib import pyplot as plt
from matplotlib import animation as anim
import numpy as np

# --- Internal Imports ---


# ---------------------------------------------------------
def animateTimeSeries( time, positions, timeSeriesSolution, model, speed=0.1, repeat=True, ylim=(-1.0,1.0) ):
    # Initialize
    values      = model.sample( timeSeriesSolution[0], positions )
    fig, ax     = plt.subplots()
    line,       = ax.plot( positions, values )
    timeLabel   = ax.text(  0.01,  
                            0.95, 
                            "t = %.3f" % time[0],
                            fontdict={  "family"    : "serif",
                                        "alpha"     : 0.2,
                                        "weight"    : "bold"} )
    plt.ylim(ylim)

    # Set callbacks
    def frameGenerator():
        for index, t in enumerate(time):
            yield ( t, model.sample( timeSeriesSolution[index], positions ) )

    def initAnim():
        line.set_ydata( [np.nan] * len(positions) )
        timeLabel.set_text( "t = %.3f" % time[0] )
        return line, timeLabel

    def stepAnim( args ):
        # Update time
        timeLabel.set_text( "t = %.3f" % args[0] )

        # Update plot
        line.set_ydata( args[1] )
        return line, timeLabel

    # Create animation
    animation = anim.FuncAnimation( fig,
                                    stepAnim,
                                    init_func=initAnim,
                                    frames=frameGenerator,
                                    interval=1.0/speed,
                                    blit=True,
                                    save_count=50,
                                    repeat_delay=1000,
                                    repeat=repeat)

    plt.show()