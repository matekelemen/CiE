project( dropmerge CXX )

# Define header path
include_directories( inc )

# Library includes
include_directories( ../../libraries/linalg/inc )

# Collect headers and sources
file( GLOB DROPMERGE_HEADER_FILES inc/*.h* )
file( GLOB DROPMERGE_SOURCE_FILES src/*.cpp )

# Build executable
add_executable( dropmerge_executable ${DROPMERGE_HEADER_FILES} ${DROPMERGE_SOURCE_FILES} )
target_link_libraries( dropmerge_executable csg )

# Build library
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
add_library( dropmerge_lib SHARED ${DROPMERGE_HEADER_FILES} src/collectnodes.cpp src/dynamictree.cpp )
if( WIN32 )
  install( TARGETS dropmerge_lib RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
else( )
  install( TARGETS dropmerge_lib LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} )
endif( )
target_link_libraries( dropmerge_lib PRIVATE csg )

# Python bindings
file( GLOB DROPMERGE_PYTHON_BINDINGS bindings/* )
pybind11_add_module( pydropmerge ${DROPMERGE_PYTHON_BINDINGS} )
target_link_libraries( pydropmerge PRIVATE dropmerge_lib )

# Copy python scripts
file( GLOB DROPMERGE_PYTHON_SCRIPTS scripts/*.py )
foreach( filename ${DROPMERGE_PYTHON_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
endforeach()

# Move to install folder
install( DIRECTORY DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
install( TARGETS dropmerge_executable RUNTIME DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
install( TARGETS pydropmerge LIBRARY DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
install( TARGETS dropmerge_lib LIBRARY DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )