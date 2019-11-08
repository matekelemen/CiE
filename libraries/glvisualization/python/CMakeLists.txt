# --------------------------------------------------
# COPY SCRIPTS
# --------------------------------------------------
# Destination folder
set( PYTHON_GLVISUALIZATION_SCRIPT_FOLDER pyglvisualization )

file( GLOB PYTHON_GLVISUALIZATION_SCRIPTS scripts/*.py )
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/${PYTHON_GLVISUALIZATION_SCRIPT_FOLDER} )
foreach( filename ${PYTHON_GLVISUALIZATION_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/${PYTHON_GLVISUALIZATION_SCRIPT_FOLDER} )
endforeach()

# --------------------------------------------------
# COPY MODULES
# --------------------------------------------------
SUBDIRLIST( moduledirectories ${CMAKE_CURRENT_SOURCE_DIR}/modules )
foreach( modulename ${moduledirectories} )
    # Create module destination folder
    install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/${modulename} )
    # Discover module files
    file( GLOB modulefiles ${CMAKE_CURRENT_SOURCE_DIR}/modules/${modulename}/*.py )
    # Copy module files
    foreach( file ${modulefiles} )
        install(FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/${modulename} )
    endforeach()
endforeach()