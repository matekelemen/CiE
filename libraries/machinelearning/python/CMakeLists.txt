# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
include_directories( ../inc )

# Collect python library files
file( GLOB PYTHON_MACHINELEARNING_CPP_FILES bindings/*.cpp )

# Add shared library of python bindings
pybind11_add_module( pymachinelearning ${PYTHON_MACHINELEARNING_CPP_FILES} )

# Link to c++ libraries
TARGET_LINK_PYTHON_BINDINGS( pymachinelearning machinelearning )

# --------------------------------------------------
# COPY SCRIPTS
# --------------------------------------------------
INSTALL_PYTHON_BINDINGS( pymachinelearning "pymachinelearning" )
INSTALL_PYTHON_MODULE( modules/pymachinelearning )
INSTALL_PYTHON_SCRIPTS( scripts )