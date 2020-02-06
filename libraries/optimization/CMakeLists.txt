project( optimization CXX )

include_directories( ../linalg/inc )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
# Gather source files into CMake variables
file( GLOB_RECURSE OPTIMIZATION_TEST_FILES test/*.cpp)

file( GLOB OPTIMIZATION_SOURCE_FILES src/*.cpp)
file( GLOB OPTIMIZATION_HEADER_FILES inc/*.h* )
file( GLOB OPTIMIZATION_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc )

# Define macro to add header and source files from subdirectories
MACRO( ADDFILES curdir )
    # Collect header and source files
    file( GLOB TEMP_HEADERS ${curdir}/inc/*.h* )
    file( GLOB TEMP_SOURCES ${curdir}/src/*.cpp )
    file( GLOB TEMP_TESTS ${curdir}/test/*.cpp )
    # Append file lists and set them in the parent scope
    list( APPEND OPTIMIZATION_INCLUDE_DIRECTORIES ${curdir}/inc )
    set( OPTIMIZATION_INCLUDE_DIRECTORIES ${OPTIMIZATION_INCLUDE_DIRECTORIES} PARENT_SCOPE )
    list( APPEND OPTIMIZATION_HEADER_FILES ${TEMP_HEADERS} )
    set( OPTIMIZATION_HEADER_FILES ${OPTIMIZATION_HEADER_FILES} PARENT_SCOPE )
    list( APPEND OPTIMIZATION_SOURCE_FILES ${TEMP_SOURCES} )
    set( OPTIMIZATION_SOURCE_FILES ${OPTIMIZATION_SOURCE_FILES} PARENT_SCOPE )
    list( APPEND OPTIMIZATION_TEST_FILES ${TEMP_TESTS} )
    set( OPTIMIZATION_TEST_FILES ${OPTIMIZATION_TEST_FILES} PARENT_SCOPE )
ENDMACRO()

SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR} )
foreach( subdirectory ${subdirs} )
    ADDFILES( ${subdirectory} )
endforeach()

add_subdirectory( gradientmethods )
add_subdirectory( populationmethods )

include_directories( ${OPTIMIZATION_INCLUDE_DIRECTORIES} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
# Add library from the collected source files. The headers are given so visual studio displays them
ADD_SHARED_LIBRARY( optimization ${OPTIMIZATION_SOURCE_FILES} ${OPTIMIZATION_HEADER_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( optimization linalg )

# Add an executable and link to the library
add_executable( optimization_testrunner ${OPTIMIZATION_TEST_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( optimization_testrunner optimization )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( optimization )
INSTALL_TEST( optimization_testrunner )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )
