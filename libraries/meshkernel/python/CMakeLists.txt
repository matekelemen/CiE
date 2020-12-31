# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( "${PROJECT_SOURCE_DIR}/inc" )

# Collect python library files
file( GLOB PYTHON_MESHKERNEL_CPP_FILES bindings/*meshkernel*.cpp )

# Add shared library of python bindings
pybind11_add_module( pymeshkernel ${PYTHON_MESHKERNEL_CPP_FILES} )

# Link to c++ libraries
TARGET_LINK_PYTHON_BINDINGS( pymeshkernel meshkernel )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_PYTHON_BINDINGS( pymeshkernel "pymeshkernel" )
INSTALL_PYTHON_MODULE( modules/pymeshkernel )
INSTALL_PYTHON_SCRIPTS( scripts )