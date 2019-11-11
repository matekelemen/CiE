cmake_minimum_required( VERSION 3.4.3 )

project( CiE CXX )

# SETTINGS

# GLOBAL MACRO DEFINITIONS
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()


# Directories to include header files from
include_directories( external/catch )

set( CMAKE_CXX_STANDARD 14 )

# If compiler is g++: Enable further  warnings and treat all warnings as errors. fPIC stands for position independent code.
if( CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror -fPIC" )
endif( CMAKE_COMPILER_IS_GNUCXX )

# Add pybind11 project (this is just the folder available at https://github.com/pybind/pybind11)
add_subdirectory( external/pybind11 )
set(PYBIND11_CPP_STANDARD -std=c++14)


# ----------------- Set up install paths -------------------------
# Changes the default install path to build/install, assuming build is the project directory
if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
endif( )

# This will add the install path to the rpath of the installed binaries to make dynamic linking work
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}" )

# Create destination folder for data
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/data )

# ---------------- Set up subprojects -----------------------------
add_subdirectory( libraries )