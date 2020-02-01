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
target_link_libraries( mathieu_stability csg )

# Move to install folder
install( DIRECTORY DESTINATION ${INSTALL_APPLICATION_PREFIX}/mathieu_stability )
install( TARGETS mathieu_stability RUNTIME DESTINATION ${INSTALL_APPLICATION_PREFIX}/mathieu_stability )

if( MSVC )
    add_custom_command(TARGET mathieu_stability POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            $<TARGET_FILE_DIR:csg>/csg.dll  
            ${INSTALL_APPLICATION_PREFIX}/mathieu_stability)
            
    add_custom_command(TARGET mathieu_stability POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            $<TARGET_FILE_DIR:linalg>/linalg.dll  
            ${INSTALL_APPLICATION_PREFIX}/mathieu_stability)
endif( )