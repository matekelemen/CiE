project( glvisualization CXX )

include_directories( ../../external/glfw/include )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
# Gather source files into CMake variables
file( GLOB_RECURSE GLVISUALIZATION_TEST_FILES test/*.cpp )

file( GLOB GLVISUALIZATION_SOURCE_FILES src/*.cpp)
file( GLOB GLVISUALIZATION_HEADER_FILES inc/*.h* )
file( GLOB GLVISUALIZATION_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc )

SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR}/packages )
foreach( subdirectory ${subdirs} )
    set( subdirectory ${CMAKE_CURRENT_SOURCE_DIR}/packages/${subdirectory} )
    file( GLOB headers ${subdirectory}/inc/*.h* )
    file( GLOB sources ${subdirectory}/src/*.cpp )
    file( GLOB tests ${subdirectory}/test/*.cpp )
    list( APPEND GLVISUALIZATION_INCLUDE_DIRECTORIES ${subdirectory}/inc )
    list( APPEND GLVISUALIZATION_HEADER_FILES ${headers} )
    list( APPEND GLVISUALIZATION_SOURCE_FILES ${sources} )
    list( APPEND GLVISUALIZATION_TEST_FILES ${tests} )
endforeach()

include_directories( ${GLVISUALIZATION_INCLUDE_DIRECTORIES} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
# Add library from the collected source files. The headers are given so visual studio displays them
ADD_SHARED_LIBRARY( glvisualization ${GLVISUALIZATION_SOURCE_FILES} ${GLVISUALIZATION_HEADER_FILES} )
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