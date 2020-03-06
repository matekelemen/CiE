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
    timeLabel   = ax.text( 0.01, 0.95, "t = " + str(time[0]) )
    plt.ylim(ylim)

    # Set callbacks
    def initAnim():
        line.set_ydata( [np.nan] * len(positions) )
        timeLabel.set_text( "t = " + str(time[0]) )
        return line, timeLabel

    def stepAnim(index):
        # Check time
        if index >= len(timeSeriesSolution):
            raise StopIteration

        # Update time
        timeLabel.set_text( "t = " + str(time[index]) )

        # Update plot
        values = model.sample( timeSeriesSolution[index], positions )
        line.set_ydata( values )
        return line, timeLabel

    # Create animation
    animation = anim.FuncAnimation( fig,
                                    stepAnim,
                                    init_func=initAnim,
                                    interval=1.0/speed,
                                    blit=True,
                                    save_count=1,
                                    repeat_delay=1000,
                                    repeat=repeat)

    plt.show()