project( dropmerge CXX )

# Define header path
include_directories( "inc" )

# Collect headers and sources
file( GLOB DROPMERGE_HEADER_FILES inc/*.h* )
file( GLOB DROPMERGE_SOURCE_FILES src/*.cpp )

# Build executable
add_executable( dropmerge_executable ${DROPMERGE_HEADER_FILES} ${DROPMERGE_SOURCE_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( dropmerge_executable csg ciegl )

# Copy python scripts
file( GLOB DROPMERGE_PYTHON_SCRIPTS scripts/*.py )
INSTALL_APPLICATION_SCRIPTS( ${DROPMERGE_PYTHON_SCRIPTS} )

# Install binaries
INSTALL_APPLICATION_EXECUTABLE( dropmerge_executable )

if( MSVC )
    add_custom_command(TARGET dropmerge_executable POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            "$<TARGET_FILE_DIR:csg>/csg.dll"  
            ${INSTALL_APPLICATION_PREFIX}/dropmerge)
            
    add_custom_command(TARGET dropmerge_executable POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            "$<TARGET_FILE_DIR:linalg>/linalg.dll"  
            ${INSTALL_APPLICATION_PREFIX}/dropmerge)

    add_custom_command(TARGET dropmerge_executable POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            "$<TARGET_FILE_DIR:ciegl>/ciegl.dll"  
            ${INSTALL_APPLICATION_PREFIX}/dropmerge)
endif( )