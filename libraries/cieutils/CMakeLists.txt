project( cieutils CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
# Gather source files into CMake variables
file( GLOB_RECURSE CIEUTILS_TEST_FILES test/*.cpp )

file( GLOB CIEUTILS_SOURCE_FILES src/*.cpp)
file( GLOB CIEUTILS_HEADER_FILES inc/*.h* )
file( GLOB CIEUTILS_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc )

SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR}/packages )
foreach( subdirectory ${subdirs} )
    set( subdirectory ${CMAKE_CURRENT_SOURCE_DIR}/packages/${subdirectory} )
    file( GLOB headers ${subdirectory}/inc/*.h* )
    file( GLOB sources ${subdirectory}/src/*.cpp )
    file( GLOB tests ${subdirectory}/test/*.cpp )
    list( APPEND CIEUTILS_INCLUDE_DIRECTORIES ${subdirectory}/inc )
    list( APPEND CIEUTILS_HEADER_FILES ${headers} )
    list( APPEND CIEUTILS_SOURCE_FILES ${sources} )
    list( APPEND CIEUTILS_TEST_FILES ${tests} )
endforeach()

include_directories( ${CIEUTILS_INCLUDE_DIRECTORIES} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
# Add library from the collected source files. The headers are given so visual studio displays them
ADD_SHARED_LIBRARY( cieutils ${CIEUTILS_SOURCE_FILES} ${CIEUTILS_HEADER_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( cieutils linalg )

# Add an executable and link to the library
add_executable( cieutils_testrunner ${CIEUTILS_TEST_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( cieutils_testrunner cieutils )
TARGET_LINK_GRAPHICS_INSTALL( cieutils_testrunner )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( cieutils )
INSTALL_TEST( cieutils_testrunner )