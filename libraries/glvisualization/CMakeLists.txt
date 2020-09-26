project( glvisualization CXX )

include_directories( "../../external/glfw/include" )
include_directories( "../../external/glm" )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES(   GLVISUALIZATION_INCLUDE_DIRECTORIES
                    GLVISUALIZATION_HEADER_FILES
                    GLVISUALIZATION_SOURCE_FILES
                    GLVISUALIZATION_TEST_FILES )

FILE( GLOB GLVISUALIZATION_TEST_DRIVER ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
LIST( INSERT GLVISUALIZATION_TEST_FILES 0 ${GLVISUALIZATION_TEST_DRIVER} )


# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
# Add library from the collected source files. The headers are given so visual studio displays them
ADD_SHARED_LIBRARY( glvisualization ${GLVISUALIZATION_SOURCE_FILES} ${GLVISUALIZATION_HEADER_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( glvisualization linalg cieutils csg )
TARGET_LINK_GRAPHICS_INSTALL( glvisualization )

# Add an executable and link to the library
add_executable( glvisualization_testrunner ${GLVISUALIZATION_TEST_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( glvisualization_testrunner glvisualization )
TARGET_LINK_GRAPHICS_INSTALL( glvisualization_testrunner )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( glvisualization )
INSTALL_TEST( glvisualization_testrunner )
INSTALL_DATA( data )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory(python)