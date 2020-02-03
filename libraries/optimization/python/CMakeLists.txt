# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( ../inc )

# Collect python library files
file( GLOB PYTHON_OPTIMIZATION_CPP_FILES bindings/*.cpp )

# Add shared library of python bindings
pybind11_add_module( pyoptimization ${PYTHON_OPTIMIZATION_CPP_FILES} )

# Link to c++ libraries
TARGET_LINK_PYTHON_BINDINGS( pyoptimization optimization )

# --------------------------------------------------
# ISTALL
# --------------------------------------------------
INSTALL_PYTHON_BINDINGS( pyoptimization "pyoptimization" )
INSTALL_PYTHON_MODULE( modules/pyoptimization )
INSTALL_PYTHON_SCRIPTS( scripts )