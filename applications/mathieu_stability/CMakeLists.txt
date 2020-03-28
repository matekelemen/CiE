project( mathieu_stability CXX )

# Define header path
include_directories( inc )

# Library includes
include_directories( ../../libraries/linalg/inc )

# Collect headers and sources
file( GLOB MATHIEU_STABILITY_HEADER_FILES inc/*.h* )
file( GLOB MATHIEU_STABILITY_SOURCE_FILES src/*.cpp )

# Build executable
add_executable( mathieu_stability ${MATHIEU_STABILITY_HEADER_FILES} ${MATHIEU_STABILITY_SOURCE_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( mathieu_stability csg glvisualization )
TARGET_LINK_GRAPHICS_INSTALL( mathieu_stability )

# Move to install folder
INSTALL_APPLICATION_EXECUTABLE( mathieu_stability )

# Copy python scripts
file( GLOB MATHIEU_PYTHON_SCRIPTS scripts/*.py )
INSTALL_APPLICATION_SCRIPTS( ${MATHIEU_PYTHON_SCRIPTS} )

if( MSVC )
    add_custom_command(TARGET mathieu_stability POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            $<TARGET_FILE_DIR:csg>/csg.dll  
            ${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME})
            
    add_custom_command(TARGET mathieu_stability POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            $<TARGET_FILE_DIR:linalg>/linalg.dll  
            ${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME})
endif( )