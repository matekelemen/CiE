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
TARGET_LINK_LIBRARIES_INSTALL( dropmerge_executable csg )

# Python bindings
file( GLOB DROPMERGE_PYTHON_BINDINGS bindings/* )
pybind11_add_module( pydropmerge ${DROPMERGE_PYTHON_BINDINGS} ${DROPMERGE_HEADER_FILES} src/collectnodes.cpp src/dynamictree.cpp )
TARGET_LINK_PYTHON_BINDINGS( pydropmerge csg )

# Copy python scripts
file( GLOB DROPMERGE_PYTHON_SCRIPTS scripts/*.py )
foreach( filename ${DROPMERGE_PYTHON_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
endforeach()

# Move to install folder
install( DIRECTORY DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
install( TARGETS dropmerge_executable RUNTIME DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
if( WIN32 )
  install( TARGETS pydropmerge DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
else( )
  install( TARGETS pydropmerge LIBRARY DESTINATION ${INSTALL_APPLICATION_PREFIX}/dropmerge )
endif( )

if( MSVC )
    add_custom_command(TARGET pydropmerge POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            $<TARGET_FILE_DIR:csg>/csg.dll  
            ${INSTALL_APPLICATION_PREFIX}/dropmerge)
endif( )

if( MSVC )
    add_custom_command(TARGET pydropmerge POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            $<TARGET_FILE_DIR:linalg>/linalg.dll  
            ${INSTALL_APPLICATION_PREFIX}/dropmerge)
endif( )