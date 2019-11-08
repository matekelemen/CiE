cmake_minimum_required( VERSION 3.4.3 )

project( pyspline CXX )

# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( ../inc )

# Collect python library headers
file( GLOB PYTHON_SPLINEKERNEL_BINDINGS_HPP bindings/splinekernel_python.cpp bindings/denseMatrixConversion.hpp bindings/sparseMatrixConversion.hpp )

# Add shared library of python bindings
pybind11_add_module( pysplinekernel ${PYTHON_SPLINEKERNEL_BINDINGS_HPP} )

# Link to c++ libraries
target_link_libraries( pysplinekernel PRIVATE splinekernel linalg )

# --------------------------------------------------
# COPY SCRIPTS
# --------------------------------------------------
# Destination folder
set( PYTHON_SPLINEKERNEL_SCRIPT_FOLDER pyspline )

file( GLOB PYTHON_SPLINEKERNEL_SCRIPTS scripts/*.py )
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PYTHON_SPLINEKERNEL_SCRIPT_FOLDER} )
foreach( filename ${PYTHON_SPLINEKERNEL_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PYTHON_SPLINEKERNEL_SCRIPT_FOLDER} )
endforeach()

# --------------------------------------------------
# COPY MODULES
# --------------------------------------------------
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pysplinekernel )
file( GLOB PYSPLINEKERNEL_MODULE_SCRIPTS modules/pysplinekernel/* )
install( TARGETS pysplinekernel LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pysplinekernel )
foreach( filename ${PYSPLINEKERNEL_MODULE_SCRIPTS} )
  install(FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pysplinekernel )
endforeach()