# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( ../inc )

# Collect python library files
file( GLOB PYTHON_FEM_CPP_FILES bindings/*.cpp )

# Add shared library of python bindings
pybind11_add_module( pyfem ${PYTHON_FEM_CPP_FILES} )

# Link to c++ libraries
TARGET_LINK_PYTHON_BINDINGS( pyfem fem )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_PYTHON_BINDINGS( pyfem "pyfem" )

SUBDIRLIST( moduleNames ${CMAKE_CURRENT_SOURCE_DIR}/modules )
foreach( module ${moduleNames} )
    INSTALL_PYTHON_MODULE( ${CMAKE_CURRENT_SOURCE_DIR}/modules/${module} )
endforeach()

INSTALL_PYTHON_SCRIPTS( scripts )