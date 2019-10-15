cmake_minimum_required( VERSION 3.4.3 )

project( CSG CXX )

# Directories to include header files from
include_directories( inc src )

# Gather source files into CMake variables
file( GLOB SOURCE_FILES src/*.cpp)
file( GLOB TEST_FILES test/*.cpp)
file( GLOB HEADER_FILES inc/*.h* )

# This enables exporting all symbols to the dll on windows
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

# ----------------- Set up spline library -------------------------

# Add library from the collected source files. The headers are given so visual studio displays them
add_library( csg SHARED ${SOURCE_FILES} ${HEADER_FILES} ) 

# specify the relative path the shared library object shall be installed to
if( WIN32 )
  install( TARGETS csg RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
else( )
  install( TARGETS csg LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} )
endif( )

target_link_libraries( csg )

# ------------------- Set up unit tests ---------------------------

# Add an executable and link to the library
add_executable( csg_testrunner ${TEST_FILES} )

target_link_libraries( csg_testrunner csg )

# specify the relative path the testrunner shall be installed to
install( TARGETS csg_testrunner RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
