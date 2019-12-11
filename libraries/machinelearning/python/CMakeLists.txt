cmake_minimum_required( VERSION 3.4.3 )

project( pymachinelearning CXX )

# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( ../inc )

# Collect python library files
file( GLOB PYTHON_MACHINELEARNING_CPP_FILES bindings/*.cpp )

# Add shared library of python bindings
pybind11_add_module( pymachinelearning ${PYTHON_MACHINELEARNING_CPP_FILES} )

# Link to c++ libraries
target_link_libraries( pymachinelearning PRIVATE machinelearning )

# --------------------------------------------------
# COPY SCRIPTS
# --------------------------------------------------
file( GLOB PYTHON_MACHINELEARNING_SCRIPTS scripts/*.py )
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
foreach( filename ${PYTHON_MACHINELEARNING_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
endforeach()

# --------------------------------------------------
# COPY MODULES
# --------------------------------------------------
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pymachinelearning )
file( GLOB PYMACHINELEARNING_MODULE_SCRIPTS modules/pymachinelearning/* )
install( TARGETS pymachinelearning LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pymachinelearning )
foreach( filename ${PYMACHINELEARNING_MODULE_SCRIPTS} )
  install(FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pymachinelearning )
endforeach()
