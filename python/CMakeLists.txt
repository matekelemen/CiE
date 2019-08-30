cmake_minimum_required( VERSION 3.4.3 )

project( python CXX )

# ------------ Set up library for C++ bindings -------------------

# Directories to include header files from
include_directories( ../splinekernel/inc )

# Add shared library of python bindings
pybind11_add_module( pysplinekernel bindings/splinekernel_python.cpp bindings/denseMatrixConversion.hpp bindings/sparseMatrixConversion.hpp )

target_link_libraries( pysplinekernel PRIVATE splinekernel linalg )

  
# -------------- Set up copying python scipts ---------------------
set( PYTHON_SCRIPT_FOLDER scripts )
set( PYSPLINEKERNEL_MODULE_FOLDER modules/pysplinekernel )
set( PYTHON_DATA_FOLDER data )

# Copy scripts
file( GLOB PYTHON_SCRIPTS ${PYTHON_SCRIPT_FOLDER}/*.py )
foreach( filename ${PYTHON_SCRIPTS} )
  install(FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX} )
endforeach()

# Copy modules
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/pysplinekernel )
file( GLOB PYSPLINEKERNEL_MODULE_SCRIPTS ${PYSPLINEKERNEL_MODULE_FOLDER}/* )
install( TARGETS pysplinekernel LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/pysplinekernel )
foreach( filename ${PYSPLINEKERNEL_MODULE_SCRIPTS} )
  install(FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/pysplinekernel )
endforeach()

# Copy data
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/data )
file( GLOB DATA_FILES ${PYTHON_DATA_FOLDER}/*.csv )
foreach( filename ${DATA_FILES} )
  install(FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/data )
endforeach()
