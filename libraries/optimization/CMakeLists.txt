cmake_minimum_required( VERSION 3.4.3 )

project( optimization CXX )

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
    # Append file lists and set them in the parent scope
    list( APPEND OPTIMIZATION_INCLUDE_DIRECTORIES ${curdir}/inc )
    set( OPTIMIZATION_INCLUDE_DIRECTORIES ${OPTIMIZATION_INCLUDE_DIRECTORIES} PARENT_SCOPE )
    list( APPEND OPTIMIZATION_HEADER_FILES ${TEMP_HEADERS} )
    set( OPTIMIZATION_HEADER_FILES ${OPTIMIZATION_HEADER_FILES} PARENT_SCOPE )
    list( APPEND OPTIMIZATION_SOURCE_FILES ${TEMP_SOURCES} )
    set( OPTIMIZATION_SOURCE_FILES ${OPTIMIZATION_SOURCE_FILES} PARENT_SCOPE )
ENDMACRO()

SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR} )
foreach( subdirectory ${subdirs} )
    # Include header directories
    include_directories( ${subdirectory}/inc )
    # Append library header and source files
    file( GLOB TEMP_HEADERS ${subdirectory}/inc/*.h* )
    file( GLOB TEMP_SOURCES ${subdirectory}/src/*.cpp )
    list( APPEND OPTIMIZATION_HEADER_FILES ${TEMP_HEADERS} )
    list( APPEND OPTIMIZATION_SOURCE_FILES ${TEMP_SOURCES} )
    # DEBUG
    #message( STATUS ${OPTIMIZATION_HEADER_FILES} )
endforeach()

add_subdirectory( gradientmethods )
add_subdirectory( populationmethods )

include_directories( ${OPTIMIZATION_INCLUDE_DIRECTORIES} )
message( STATUS ${OPTIMIZATION_HEADER_FILES} )
message( STATUS ${OPTIMIZATION_SOURCE_FILES} )

# This enables exporting all symbols to the dll on windows
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

# ----------------- Set up libraries -------------------------

# Add library from the collected source files. The headers are given so visual studio displays them
add_library( optimization SHARED ${OPTIMIZATION_SOURCE_FILES} ${OPTIMIZATION_HEADER_FILES} ) 

# specify the relative path the shared library object shall be installed to
if( WIN32 )
  install( TARGETS optimization RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
else( )
  install( TARGETS optimization LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} )
endif( )

target_link_libraries( optimization )

# ------------------- Set up unit tests ---------------------------

# Add an executable and link to the library
add_executable( optimization_testrunner ${OPTIMIZATION_TEST_FILES} )

target_link_libraries( optimization_testrunner optimization )

# specify the relative path the testrunner shall be installed to
install( TARGETS optimization_testrunner RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Add python stuff
add_subdirectory( python )