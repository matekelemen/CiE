cmake_minimum_required( VERSION 3.4.3 )

project( CSG CXX )

# Initialize file lists
file( GLOB CSG_SOURCE_FILES src/*.cpp)
file( GLOB_RECURSE CSG_TEST_FILES test/*.cpp)
file( GLOB CSG_HEADER_FILES inc/*.h* )
file( GLOB CSG_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc )

# Define macro to add header and source files from subdirectories
MACRO( ADDFILES curdir )
    # Collect header and source files
    file( GLOB TEMP_HEADERS ${curdir}/inc/*.h* )
    file( GLOB TEMP_SOURCES ${curdir}/src/*.cpp )
    file( GLOB TEMP_TESTS ${curdir}/test/*.cpp )
    # Append file lists and set them in the parent scope
    list( APPEND CSG_INCLUDE_DIRECTORIES ${curdir}/inc )
    set( CSG_INCLUDE_DIRECTORIES ${CSG_INCLUDE_DIRECTORIES} PARENT_SCOPE )
    list( APPEND CSG_HEADER_FILES ${TEMP_HEADERS} )
    set( CSG_HEADER_FILES ${CSG_HEADER_FILES} PARENT_SCOPE )
    list( APPEND CSG_SOURCE_FILES ${TEMP_SOURCES} )
    set( CSG_SOURCE_FILES ${CSG_SOURCE_FILES} PARENT_SCOPE )
    list( APPEND CSG_TEST_FILES ${TEMP_TESTS} )
    set( CSG_TEST_FILES ${CSG_TEST_FILES} PARENT_SCOPE )
ENDMACRO()

SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR} )
foreach( subdirectory ${subdirs} )
    ADDFILES( ${subdirectory} )
endforeach()

# This enables exporting all symbols to the dll on windows
set( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )

# ----------------- Set up spline library -------------------------

# Add library from the collected source files. The headers are given so visual studio displays them
add_library( csg SHARED ${CSG_SOURCE_FILES} ${CSG_HEADER_FILES} ) 

# specify the relative path the shared library object shall be installed to
if( WIN32 )
  install( TARGETS csg RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
else( )
  install( TARGETS csg LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX} )
endif( )

target_link_libraries( csg )

# ------------------- Set up unit tests ---------------------------

# Add an executable and link to the library
add_executable( csg_testrunner ${CSG_TEST_FILES} )

target_link_libraries( csg_testrunner csg )

# specify the relative path the testrunner shall be installed to
install( TARGETS csg_testrunner RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
