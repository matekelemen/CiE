cmake_minimum_required( VERSION 3.4.3 )

project( pymesh CXX )

# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( ../inc )

# Collect python library files
file( GLOB PYTHON_MESHKERNEL_CPP_FILES bindings/*meshkernel*.cpp )

# Add shared library of python bindings
pybind11_add_module( pymeshkernel ${PYTHON_MESHKERNEL_CPP_FILES} )

# Link to c++ libraries
target_link_libraries( pymeshkernel PRIVATE meshkernel )

# --------------------------------------------------
# COPY SCRIPTS
# --------------------------------------------------
file( GLOB PYTHON_MESHKERNEL_SCRIPTS scripts/*.py )
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
foreach( filename ${PYTHON_MESHKERNEL_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
endforeach()

# --------------------------------------------------
# COPY MODULES
# --------------------------------------------------
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pymeshkernel )
file( GLOB PYSMESHKERNEL_MODULE_SCRIPTS modules/pymeshkernel/* )
install( TARGETS pymeshkernel LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pymeshkernel )
foreach( filename ${PYSMESHKERNEL_MODULE_SCRIPTS} )
  install(FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pymeshkernel )
endforeach()
