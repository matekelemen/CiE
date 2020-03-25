project( machinelearning CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
file( GLOB_RECURSE MACHINELEARNING_TEST_FILES test/*.cpp)
file( GLOB MACHINELEARNING_SOURCE_FILES src/*.cpp)
file( GLOB MACHINELEARNING_HEADER_FILES inc/*.h* )
file( GLOB MACHINELEARNING_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/inc" )

# Define macro to add header and source files from subdirectories
MACRO( ADDFILES curdir )
    # Collect header and source files
    file( GLOB TEMP_HEADERS ${curdir}/inc/*.h* )
    file( GLOB TEMP_SOURCES ${curdir}/src/*.cpp )
    file( GLOB TEMP_TESTS ${curdir}/test/*.cpp )
    # Append file lists and set them in the parent scope
    list( APPEND MACHINELEARNING_INCLUDE_DIRECTORIES "${curdir}/inc" )
    set( MACHINELEARNING_INCLUDE_DIRECTORIES ${MACHINELEARNING_INCLUDE_DIRECTORIES} PARENT_SCOPE )
    list( APPEND MACHINELEARNING_HEADER_FILES ${TEMP_HEADERS} )
    set( MACHINELEARNING_HEADER_FILES ${MACHINELEARNING_HEADER_FILES} PARENT_SCOPE )
    list( APPEND MACHINELEARNING_SOURCE_FILES ${TEMP_SOURCES} )
    set( MACHINELEARNING_SOURCE_FILES ${MACHINELEARNING_SOURCE_FILES} PARENT_SCOPE )
    list( APPEND MACHINELEARNING_TEST_FILES ${TEMP_TESTS} )
    set( MACHINELEARNING_TEST_FILES ${MACHINELEARNING_TEST_FILES} PARENT_SCOPE )
ENDMACRO()

SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR} )
foreach( subdirectory ${subdirs} )
    ADDFILES( ${subdirectory} )
endforeach()

add_subdirectory( classification )
add_subdirectory( regression )

include_directories( ${MACHINELEARNING_INCLUDE_DIRECTORIES} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY(  machinelearning SHARED ${MACHINELEARNING_SOURCE_FILES} ${MACHINELEARNING_HEADER_FILES} )

add_executable( machinelearning_testrunner ${MACHINELEARNING_TEST_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( machinelearning_testrunner machinelearning )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( machinelearning )
INSTALL_TEST( machinelearning_testrunner )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )
