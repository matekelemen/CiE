# --- Python Imports ---
import itertools
import numpy as np

# ---------------------------------------------------------
def binaryPermutation( numberOfBits ):
    return np.asarray( [list(i) for i in itertools.product([0,1], repeat=numberOfBits)], dtype=np.int)


def oneHotArray( numberOfBits ):
    return np.asarray( [ [int(i==j) for i in range(numberOfBits)] for j in range(numberOfBits) ], dtype=np.int)