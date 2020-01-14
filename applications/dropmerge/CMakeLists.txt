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

# Python bindings
file( GLOB DROPMERGE_PYTHON_BINDINGS bindings/* )
pybind11_add_module( pydropmerge ${DROPMERGE_PYTHON_BINDINGS} )
target_link_libraries( pydropmerge PRIVATE csg )

# Move to install folder
install( DIRECTORY DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
install( TARGETS dropmerge_executable RUNTIME DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
install( TARGETS pydropmerge LIBRARY DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )