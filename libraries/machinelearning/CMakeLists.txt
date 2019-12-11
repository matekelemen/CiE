cmake_minimum_required( VERSION 3.4.3 )

project( machinelearning CXX )

include_directories( ../linalg/inc )

# Gather source files into CMake variables
file( GLOB_RECURSE MACHINELEARNING_TEST_FILES test/*.cpp)

file( GLOB MACHINELEARNING_SOURCE_FILES src/*.cpp)
file( GLOB MACHINELEARNING_HEADER_FILES inc/*.h* )
file( GLOB MACHINELEARNING_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc )

# Define macro to add header and source files from subdirectories
MACRO( ADDFILES curdir )
    # Collect header and source files
    file( GLOB TEMP_HEADERS ${curdir}/inc/*.h* )
    file( GLOB TEMP_SOURCES ${curdir}/src/*.cpp )
    file( GLOB TEMP_TESTS ${curdir}/test/*.cpp )
    # Append file lists and set them in the parent scope
    list( APPEND MACHINELEARNING_INCLUDE_DIRECTORIES ${curdir}/inc )
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
#message( STATUS ${MACHINELEARNING_HEADER_FILES} )
#message( STATUS ${MACHINELEARNING_SOURCE_FILES} )
#message( STATUS ${MACHINELEARNING_TEST_FILES} )

# This enables exporting all symbols to the dll on windows
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

# ----------------- Set up libraries -------------------------

# Add library from the collected source files. The headers are given so visual studio displays them
add_library( machinelearning SHARED ${MACHINELEARNING_SOURCE_FILES} ${MACHINELEARNING_HEADER_FILES} ) 

# specify the relative path the shared library object shall be installed to
if( WIN32 )
  install( TARGETS machinelearning RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
else( )
  install( TARGETS machinelearning LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} )
endif( )

target_link_libraries( machinelearning )

# ------------------- Set up unit tests ---------------------------

# Add an executable and link to the library
add_executable( machinelearning_testrunner ${MACHINELEARNING_TEST_FILES} )

target_link_libraries( machinelearning_testrunner machinelearning )

# specify the relative path the testrunner shall be installed to
install( TARGETS machinelearning_testrunner RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Add python stuff
add_subdirectory( python )
