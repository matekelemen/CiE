project( CSG CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES( HEADERS
                  SOURCES
                  TESTS )

FILE( GLOB CSG_TEST_DRIVER ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
LIST( INSERT TESTS 0 ${CSG_TEST_DRIVER} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY( csg ${SOURCES} )
TARGET_LINK_LIBRARIES_INSTALL( csg linalg pugixml )
INSTALL_LIBRARY( csg )

ADD_TEST_EXECUTABLE( csg_testrunner ${TESTS} ${HEADERS} )
TARGET_LINK_LIBRARIES_INSTALL( csg_testrunner csg linalg )

# ---------------------------------------------------------
# WINDOWS
# ---------------------------------------------------------
# To be able to debug in Windows we have to copy the desired library to the project debug folder.
# This adds a post build command that after building the executable copies the library over.
#if( MSVC )
#    add_custom_command(TARGET csg_testrunner POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#            "$<TARGET_FILE_DIR:linalg>/linalg.dll"
#            $<TARGET_FILE_DIR:csg_testrunner>)
#endif( )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )