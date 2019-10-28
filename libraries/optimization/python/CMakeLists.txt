cmake_minimum_required( VERSION 3.4.3 )

project( pyoptimization CXX )

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
target_link_libraries( pyoptimization PRIVATE optimization )

# --------------------------------------------------
# COPY SCRIPTS
# --------------------------------------------------
file( GLOB PYTHON_OPTIMIZATION_SCRIPTS scripts/*.py )
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
foreach( filename ${PYTHON_OPTIMIZATION_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
endforeach()

# --------------------------------------------------
# COPY MODULES
# --------------------------------------------------
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pyoptimization )
file( GLOB PYOPTIMIZATION_MODULE_SCRIPTS modules/pyoptimization/* )
install( TARGETS pyoptimization LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pyoptimization )
foreach( filename ${PYOPTIMIZATION_MODULE_SCRIPTS} )
  install(FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pyoptimization )
endforeach()
