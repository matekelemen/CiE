cmake_minimum_required( VERSION 3.7.0 )

project( CiE CXX )

set( CMAKE_CXX_STANDARD 17 )


# ---------------------------------------------------------
# OPTIONS
OPTION( INSTALL_TESTS "Install unit-and system tests" ON )

# ---------------------------------------------------------
# INSTALL PATH SETUP
# Changes the default install path to build/install, assuming build is the project directory
if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set ( CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
endif( )

# This will add the install path to the rpath of the installed binaries to make dynamic linking work
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}:$ORIGIN" )
#SET( BUILD_RPATH_USE_ORIGIN TRUE )

# ---------------------------------------------------------
# CREATE DESTINATION FOLDERS
install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Executable applications
set( INSTALL_APPLICATION_PREFIX ${CMAKE_INSTALL_PREFIX}/applications )
install( DIRECTORY DESTINATION ${INSTALL_APPLICATION_PREFIX} )

# Dynamic libraries
set( INSTALL_LIBRARY_PREFIX ${CMAKE_INSTALL_PREFIX}/libraries )
install( DIRECTORY DESTINATION ${INSTALL_LIBRARY_PREFIX} )

# Executable tests
set( INSTALL_TEST_PREFIX ${CMAKE_INSTALL_PREFIX}/tests )
install( DIRECTORY DESTINATION ${INSTALL_TEST_PREFIX} )

# Data
set( INSTALL_DATA_PREFIX ${CMAKE_INSTALL_PREFIX}/data )
install( DIRECTORY DESTINATION ${INSTALL_DATA_PREFIX} )

# Python bindings
set( INSTALL_PYTHON_MODULES_PREFIX ${CMAKE_INSTALL_PREFIX}/modules )
install( DIRECTORY DESTINATION ${INSTALL_PYTHON_MODULES_PREFIX} )

# Python scripts
set( INSTALL_PYTHON_SCRIPTS_PREFIX ${CMAKE_INSTALL_PREFIX}/scripts )
install( DIRECTORY DESTINATION ${INSTALL_PYTHON_SCRIPTS_PREFIX} )

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


MACRO( INSTALL_LIBRARY target )
  if( WIN32 )
    install( TARGETS ${target} RUNTIME DESTINATION ${INSTALL_LIBRARY_PREFIX} )
  else( )
    install( TARGETS ${target} LIBRARY DESTINATION ${INSTALL_LIBRARY_PREFIX} )
  endif( )
ENDMACRO()


MACRO( INSTALL_APPLICATION target )
  install( TARGETS ${target} RUNTIME DESTINATION ${INSTALL_APPLICATION_PREFIX} )
ENDMACRO()


MACRO( INSTALL_TEST target )
  if( INSTALL_TESTS )
    install( TARGETS ${target} RUNTIME DESTINATION ${INSTALL_TEST_PREFIX} )
  endif()
ENDMACRO()


MACRO( INSTALL_PYTHON_BINDINGS target moduleName )
  install( DIRECTORY DESTINATION ${INSTALL_PYTHON_MODULES_PREFIX}/${moduleName} )
  if( WIN32 )
  install( TARGETS ${target} RUNTIME DESTINATION ${INSTALL_PYTHON_MODULES_PREFIX}/${moduleName} )
  else( )
  install( TARGETS ${target} LIBRARY DESTINATION ${INSTALL_PYTHON_MODULES_PREFIX}/${moduleName} )
  endif( )
ENDMACRO()


MACRO( INSTALL_PYTHON_MODULE moduleFolder )
  if( ${ARGC} EQUAL 1 )
    install( DIRECTORY ${moduleFolder} DESTINATION ${INSTALL_PYTHON_MODULES_PREFIX} )
  elseif( ${ARGC} EQUAL 2 )
    set( moduleName ${ARGV1} )
    install( DIRECTORY ${moduleFolder}/ DESTINATION ${INSTALL_PYTHON_MODULES_PREFIX}/${moduleName} )
  endif()
ENDMACRO()


MACRO( INSTALL_PYTHON_SCRIPTS scriptFolder )
  install( DIRECTORY ${scriptFolder}/ DESTINATION ${INSTALL_PYTHON_SCRIPTS_PREFIX}/${PROJECT_NAME} )
ENDMACRO()


MACRO( INSTALL_DATA dataFolder )
  install( DIRECTORY ${dataFolder}/ DESTINATION ${INSTALL_DATA_PREFIX}/${PROJECT_NAME} )
ENDMACRO()


FUNCTION( TARGET_LINK_PYTHON_BINDINGS target )
  # Remove target from argument list
  LIST( REMOVE_ITEM ARGV ${target} )
  # Stackoverflow magic
  set_target_properties( ${target} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE )
  target_link_libraries( ${target} PRIVATE ${ARGV} )
  
  if(UNIX AND NOT APPLE)
    set_target_properties( ${target} PROPERTIES INSTALL_RPATH ${INSTALL_LIBRARY_PREFIX} )
  endif()

  set_target_properties( ${target} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDFUNCTION()


FUNCTION( TARGET_LINK_LIBRARIES_INSTALL target )
  # Remove target from argument list
  LIST( REMOVE_ITEM ARGV ${target} )
  # Stackoverflow magic
  set_target_properties( ${target} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE )
  target_link_libraries( ${target} ${ARGV} )
  
  if(UNIX AND NOT APPLE)
    set_target_properties( ${target} PROPERTIES INSTALL_RPATH ${INSTALL_LIBRARY_PREFIX} )
  endif()
  
  set_target_properties( ${target} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDFUNCTION()

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