cmake_minimum_required( VERSION 3.4.3 )

project( meshkernel CXX )

# Directories to include header files from
include_directories( inc )

# Gather source files into CMake variables
file( GLOB SOURCE_FILES src/*.cpp)
file( GLOB TEST_FILES test/*.cpp)
file( GLOB HEADER_FILES inc/*.h* )

# This enables exporting all symbols to the dll on windows
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

# ----------------- Set up spline library -------------------------

# Add library from the collected source files. The headers are given so visual studio displays them
add_library( meshkernel SHARED ${SOURCE_FILES} ${HEADER_FILES} ) 
 
# specify the relative path the shared library object shall be installed to
if( WIN32 )
  install( TARGETS meshkernel RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
else( )
  install( TARGETS meshkernel LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} )
endif( )

# ------------------- Set up unit tests ---------------------------

# Add an executable and link to the library
add_executable( meshkernel_testrunner ${TEST_FILES} ${HEADER_FILES} )

target_link_libraries( meshkernel_testrunner meshkernel )

# specify the relative path the testrunner shall be installed to
install( TARGETS meshkernel_testrunner RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Add python bindings and scripts
add_subdirectory( python )
