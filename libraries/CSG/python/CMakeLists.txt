cmake_minimum_required( VERSION 3.4.3 )

project( pycsg CXX )

# --------------------------------------------------
# SET UP BINDINGS
# --------------------------------------------------
# Include headers
include_directories( ../inc )

# Collect python library files
file( GLOB PYTHON_PYCSG_CPP_FILES bindings/*.cpp )

# Add shared library of python bindings
pybind11_add_module( pycsg ${PYTHON_PYCSG_CPP_FILES} )

# Link to c++ libraries
target_link_libraries( pycsg PRIVATE csg )

# --------------------------------------------------
# COPY SCRIPTS
# --------------------------------------------------
file( GLOB PYTHON_CSG_SCRIPTS scripts/*.py )
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
foreach( filename ${PYTHON_CSG_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME} )
endforeach()

# --------------------------------------------------
# COPY MODULES
# --------------------------------------------------
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pycsg )
file( GLOB PYCSG_MODULE_SCRIPTS modules/pycsg/* )
install( TARGETS pycsg LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pycsg )
foreach( filename ${PYCSG_MODULE_SCRIPTS} )
  install(FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/pycsg )
endforeach()
