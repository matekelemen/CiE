cmake_minimum_required( VERSION 3.7.0 )

project( CiE CXX )

set( CMAKE_CXX_STANDARD 20 )

# ---------------------------------------------------------
# OPTIONS
# ---------------------------------------------------------
include( cmake/options.cmake )

# ---------------------------------------------------------
# INSTALL PATH SETUP
# ---------------------------------------------------------
# Changes the default install path to build/install, assuming build is the project directory
if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
endif( )

# This will add the install path to the rpath of the installed binaries to make dynamic linking work
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}:$ORIGIN" )
#SET( BUILD_RPATH_USE_ORIGIN TRUE )


# ---------------------------------------------------------
# WINDOWS
# ---------------------------------------------------------
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if( WIN32 )
    set( EXECUTABLE_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/bin )
    set( LIBRARY_OUTPUT_PATH ${CMAKE_INSTALL_PREFIX}/bin )
endif(  )

# ---------------------------------------------------------
# CREATE INSTALL DESTINATION FOLDERS
# ---------------------------------------------------------
include( cmake/vars_install.cmake )

# ---------------------------------------------------------
# GLOBAL MACRO DEFINITIONS
# ---------------------------------------------------------
include( cmake/macro_global.cmake )
include( cmake/macro_install.cmake )
include( cmake/macro_link.cmake )
include( cmake/macro_add.cmake )

# Copy CMAKE variables in global header
configure_file( cmake/cmake_variables.hpp.in ${CMAKE_BINARY_DIR}/generated/cmake_variables.hpp )
include_directories( ${CMAKE_BINARY_DIR}/generated )

# ---------------------------------------------------------
# COMPILER FLAGS
# ---------------------------------------------------------
include( cmake/compiler.cmake )

# ---------------------------------------------------------
# EXTERNAL
# ---------------------------------------------------------
include_directories( external/catch )

# Add pybind11 project (this is just the folder available at https://github.com/pybind/pybind11)
set(PYBIND11_CPP_STANDARD -std=c++17)
add_subdirectory( external/pybind11 )

if( CIE_USE_CPP_GRAPHICS )
    if( WIN32 )
        set( BUILD_SHARED_LIBS ON )
    endif()

    find_package( OpenGL REQUIRED )
    add_subdirectory( external/glfw )
    add_subdirectory( external/glm )
    add_subdirectory( external/glad )

    if( NOT WIN32 )
        set( BUILD_SHARED_LIBS ON "Always build shared libraries" CACHE BOOL "Always build shared libraries" FORCE )
    endif()
endif(  )

# ---------------------------------------------------------
# BUILD LIBRARIES AND TESTS
# ---------------------------------------------------------
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/libraries" )
add_subdirectory( libraries )

# ---------------------------------------------------------
# BUILD APPLICATIONS
# ---------------------------------------------------------
add_subdirectory( applications )

# ---------------------------------------------------------
# TESTRUNNER
# ---------------------------------------------------------
add_custom_target( testrunner
                    COMMAND ${CMAKE_COMMAND} 
                        "-DVAR=${INSTALL_TEST_PREFIX}" 
                        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/testrunner.cmake" )