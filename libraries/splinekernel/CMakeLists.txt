project( splinekernel CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
file( GLOB SPLINEKERNEL_SOURCE_FILES src/*.cpp)
file( GLOB SPLINEKERNEL_TEST_FILES test/*.cpp)
file( GLOB SPLINEKERNEL_HEADER_FILES inc/*.h* )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY( splinekernel ${SPLINEKERNEL_SOURCE_FILES} ${SPLINEKERNEL_HEADER_FILES} ) 
TARGET_LINK_LIBRARIES_INSTALL( splinekernel linalg )
INSTALL_LIBRARY( splinekernel )

ADD_TEST_EXECUTABLE( splinekernel_testrunner ${SPLINEKERNEL_TEST_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( splinekernel_testrunner splinekernel linalg )

# ---------------------------------------------------------
# WINDOWS
# ---------------------------------------------------------
# To be able to debug in Windows we have to copy the linalg library to the splinekernel debug folder.
# This adds a post build command that after building splinekernel_testrunner copies linalg.dll over.
#if( MSVC )
#    add_custom_command(TARGET splinekernel_testrunner POST_BUILD        	
#        COMMAND ${CMAKE_COMMAND} -E copy 					
#            $<TARGET_FILE_DIR:linalg>/linalg.dll  
#            $<TARGET_FILE_DIR:splinekernel_testrunner>)
#endif( )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )