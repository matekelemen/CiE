project( glvisualization CXX )

# --------------------------------------------------
# GATHER FILES


# --------------------------------------------------
# COPY DATA
# Create directory for project data
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/data/${PROJECT_NAME} )
# Discover data directories
SUBDIRLIST( datadirectories ${CMAKE_CURRENT_SOURCE_DIR}/data )
foreach( directory ${datadirectories} )
    # Collect data files
    file( GLOB datafiles ${CMAKE_CURRENT_SOURCE_DIR}/data/${directory}/* )
    # Copy files
    foreach( file ${datafiles} )
        install( FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/data/${PROJECT_NAME}/${directory} )
    endforeach()
endforeach()

# Handle python-related code
add_subdirectory(python)