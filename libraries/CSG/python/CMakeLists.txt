# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( ../inc )

# Collect python library files
file( GLOB PYTHON_CSG_BINDINGS bindings/* )

# Add shared library of python bindings
pybind11_add_module( pycsg ${PYTHON_CSG_BINDINGS} )

# Link to c++ libraries
TARGET_LINK_PYTHON_BINDINGS( pycsg csg )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_PYTHON_BINDINGS( pycsg "pycsg" )
INSTALL_PYTHON_MODULE( modules/pycsg )
INSTALL_PYTHON_SCRIPTS( scripts )