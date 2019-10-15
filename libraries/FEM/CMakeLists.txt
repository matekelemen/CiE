cmake_minimum_required( VERSION 3.4.3 )

project( FEM CXX )

# Directories to include header files from
include_directories( inc )

# Gather source files into CMake variables
file( GLOB FEM_SOURCE_FILES src/*.cpp)
file( GLOB FEM_TEST_FILES test/*.cpp)
file( GLOB FEM_HEADER_FILES inc/*.h* )

# This enables exporting all symbols to the dll on windows
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

# ----------------- Set up libraries -------------------------

# Add library from the collected source files. The headers are given so visual studio displays them
add_library( fem SHARED ${FEM_SOURCE_FILES} ${FEM_HEADER_FILES} ) 

# specify the relative path the shared library object shall be installed to
if( WIN32 )
  install( TARGETS fem RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
else( )
  install( TARGETS fem LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} )
endif( )

target_link_libraries( fem )

# ------------------- Set up unit tests ---------------------------

# Add an executable and link to the library
add_executable( fem_testrunner ${FEM_TEST_FILES} )

target_link_libraries( fem_testrunner fem )

# specify the relative path the testrunner shall be installed to
install( TARGETS fem_testrunner RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )

# ------------------- Copy python scripts ------------------------
file( GLOB FEM_PYTHON_SCRIPTS python/scripts/*.py )
foreach( filename ${FEM_PYTHON_SCRIPTS} )
    install( FILES ${filename} DESTINATION ${CMAKE_INSTALL_PREFIX}/FEM )
endforeach()

# Add python stuff
add_subdirectory( python )