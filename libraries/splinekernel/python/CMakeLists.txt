# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( ../inc )

# Collect python library headers
file( GLOB PYTHON_SPLINEKERNEL_BINDINGS bindings/* )

# Add shared library of python bindings
pybind11_add_module( pysplinekernel ${PYTHON_SPLINEKERNEL_BINDINGS} )

# Link to c++ libraries
TARGET_LINK_PYTHON_BINDINGS( pysplinekernel splinekernel )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_PYTHON_BINDINGS( pysplinekernel "pysplinekernel" )
INSTALL_PYTHON_MODULE( modules/pysplinekernel )
INSTALL_PYTHON_SCRIPTS( scripts )