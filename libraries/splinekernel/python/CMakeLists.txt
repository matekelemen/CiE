# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( "${PROJECT_SOURCE_DIR}/inc" )

# Collect python library headers
file( GLOB PYTHON_SPLINEKERNEL_BINDINGS bindings/* )

# Add shared library of python bindings
pybind11_add_module( pysplinekernel ${PYTHON_SPLINEKERNEL_BINDINGS} )

# Link to c++ libraries
TARGET_LINK_PYTHON_BINDINGS( pysplinekernel splinekernel linalg )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_PYTHON_BINDINGS( pysplinekernel "pysplinekernel" )
INSTALL_PYTHON_MODULE( "${CMAKE_CURRENT_SOURCE_DIR}/modules/pysplinekernel" )
INSTALL_PYTHON_SCRIPTS( scripts )