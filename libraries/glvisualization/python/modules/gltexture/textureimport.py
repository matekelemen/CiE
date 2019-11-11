# --- Python imports ---
import numpy as np
from PIL import Image

# --- OS imports ---
import os

# -----------------------------------------------------------
# Define texture folder path
textureFolderPath   = os.path.abspath( os.path.join( os.path.realpath(__file__), os.path.pardir ) + '/../../data/glvisualization/textures' )


# -----------------------------------------------------------
def loadTexture(                filePath, 
                                format='rgb', 
                                dataType='float' ):
    # Set format
    if format is 'rgb':
        texelSize   = 3
    elif format is 'grayscale':
        texelSize   = 1
    elif format is 'rgba':
        texelSize   = 4
    else:
        raise ValueError('Invalid texture format!')
    
    # Set data type
    if dataType is 'float':
        dataType    = np.float32
        norm        = 255.0
    elif dataType is 'uint':
        dataType    = np.uint8
        norm        = 1
    else:
        raise ValueError('Invalid data type!')

    # Load and format
    return np.asarray( np.array(Image.open(filePath))/norm, dtype=dataType )[:,:,:texelSize]