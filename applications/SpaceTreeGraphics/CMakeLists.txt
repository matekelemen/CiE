project( SpaceTreeGraphics CXX )

# Define header path
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/inc" )

# Collect headers and sources
file( GLOB HEADERS inc/*.h* )
file( GLOB SOURCES src/*.cpp )
file( GLOB DRIVERS drivers/*.cpp )

# Build executable
foreach( driver ${DRIVERS} )
    get_filename_component( executable_name ${driver} NAME_WE )
    message( STATUS "Add executable: ${executable_name}" )
    add_executable( ${executable_name} ${HEADERS} ${SOURCES} ${driver} )
    TARGET_LINK_LIBRARIES_INSTALL( ${executable_name} csg ciegl )

    INSTALL_APPLICATION_EXECUTABLE( ${executable_name} )
endforeach()

# Copy python scripts
file( GLOB PYTHON_SCRIPTS scripts/*.py )
INSTALL_APPLICATION_SCRIPTS( ${PYTHON_SCRIPTS} )


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