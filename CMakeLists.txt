cmake_minimum_required( VERSION 3.7.0 )

project( CiE CXX )

<<<<<<< HEAD
# ----------------- Set up install paths -------------------------
=======
set( CMAKE_CXX_STANDARD 17 )


# ---------------------------------------------------------
# INSTALL PATH SETUP
>>>>>>> b52f03d67006ed4794acbe263402da9a8e3885fb
# Changes the default install path to build/install, assuming build is the project directory
if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
endif( )

# This will add the install path to the rpath of the installed binaries to make dynamic linking work
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}" )

# ---------------------------------------------------------
# CREATE DESTINATION FOLDERS

# Create destination folder for executable applications
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/applications )
set( INSTALL_APPLICATION_PREFIX ${CMAKE_INSTALL_PREFIX}/applications )

# Create destination folder for libraries
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/libraries )
set( INSTALL_LIBRARY_PREFIX ${CMAKE_INSTALL_PREFIX}/libraries )

# Create destination folder for data
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/data )
set( INSTALL_DATA_PREFIX ${CMAKE_INSTALL_PREFIX}/data )

# ---------------------------------------------------------
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

MACRO( LIBRARY_INSTALL_PATH path )
    SET( ${path} "${INSTALL_LIBRARY_PREFIX}/${PROJECT_NAME}" )
ENDMACRO()

MACRO( APPLICATION_INSTALL_PATH path )
    SET( ${path} "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}" )
ENDMACRO()

MACRO( DATA_INSTALL_PATH path )
    SET( ${path} "${INSTALL_DATA_PREFIX}/${PROJECT_NAME}" )
ENDMACRO()

# ---------------------------------------------------------
# EXTERNAL
include_directories( external/catch )

# If compiler is g++: Enable further  warnings and treat all warnings as errors. fPIC stands for position independent code.
if( CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror -fPIC" )
endif( CMAKE_COMPILER_IS_GNUCXX )

# Add pybind11 project (this is just the folder available at https://github.com/pybind/pybind11)
add_subdirectory( external/pybind11 )
set(PYBIND11_CPP_STANDARD -std=c++17)

# ---------------------------------------------------------
# Build libraries and tests
add_subdirectory( libraries )

# Build applications
add_subdirectory( applications )