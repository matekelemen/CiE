project( ciegl CXX )

include_directories( "../../external/glfw/include" )
include_directories( "../../external/glm" )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES(   CIEGL_HEADER_FILES
                    CIEGL_SOURCE_FILES
                    CIEGL_TEST_FILES )

FILE( GLOB CIEGL_TEST_DRIVER ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
LIST( INSERT CIEGL_TEST_FILES 0 ${CIEGL_TEST_DRIVER} )


# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
# Add library from the collected source files. The headers are given so visual studio displays them
ADD_SHARED_LIBRARY( ciegl ${CIEGL_SOURCE_FILES} ${CIEGL_HEADER_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( ciegl linalg cieutils csg )
TARGET_LINK_GRAPHICS_INSTALL( ciegl )

# Add an executable and link to the library
add_executable( ciegl_testrunner ${CIEGL_TEST_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( ciegl_testrunner ciegl )
TARGET_LINK_GRAPHICS_INSTALL( ciegl_testrunner )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( ciegl )
INSTALL_TEST( ciegl_testrunner )
INSTALL_DATA( data )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory(python)