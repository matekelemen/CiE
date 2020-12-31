project( ciegl CXX )

include_directories( "../../external/glfw/include" )
include_directories( "../../external/glm" )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES( HEADERS
                  SOURCES
                  TEST_SOURCES )

FILE( GLOB TEST_DRIVER ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
LIST( INSERT TEST_SOURCES 0 ${TEST_DRIVER} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
# Add library from the collected source files. The headers are given so visual studio displays them
ADD_SHARED_LIBRARY( ciegl ${SOURCES} )
TARGET_LINK_LIBRARIES_INSTALL( ciegl linalg cieutils csg meshkernel )
TARGET_LINK_GRAPHICS_INSTALL( ciegl )

# Add an executable and link to the library
add_executable( ciegl_testrunner ${TEST_SOURCES} )
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