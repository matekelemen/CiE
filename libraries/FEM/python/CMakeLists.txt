cmake_minimum_required( VERSION 3.4.3 )

project( pyfem CXX )

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
target_link_libraries( pyfem PRIVATE fem )

# --------------------------------------------------
# COPY SCRIPTS
# --------------------------------------------------
file( GLOB PYTHON_FEM_SCRIPTS scripts/*.py )
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
foreach( filename ${PYTHON_FEM_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
endforeach()

# --------------------------------------------------
# COPY MODULES
# --------------------------------------------------
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pyfem )
file( GLOB PYFEM_MODULE_SCRIPTS modules/pyfem/* )
install( TARGETS pyfem LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pyfem )
foreach( filename ${PYFEM_MODULE_SCRIPTS} )
  install(FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pyfem )
endforeach()
