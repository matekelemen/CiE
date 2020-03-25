project( linalg CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
file( GLOB_RECURSE LINALG_TEST_SOURCE_FILES test/*.cpp )
file( GLOB LINALG_LIBRARY_SOURCE_FILES src/*.cpp )
file( GLOB LINALG_HEADER_FILES inc/*.h* )

SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR} )
foreach( subdirectory ${subdirs} )
    # Include header directories
    include_directories( "${subdirectory}/inc" )
    # Append library header and source files
    file( GLOB TEMP_HEADERS ${subdirectory}/inc/*.h* )
    file( GLOB TEMP_SOURCES ${subdirectory}/src/*.cpp )
    file( GLOB TEMP_TESTS ${subdirectory}/test/*.cpp )
    list( APPEND LINALG_HEADER_FILES ${TEMP_HEADERS} )
    list( APPEND LINALG_LIBRARY_SOURCE_FILES ${TEMP_SOURCES} )
    list( APPEND LINALG_TEST_SOURCE_FILES ${TEMP_TESTS} )
endforeach()

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/inc" )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY(  linalg ${LINALG_LIBRARY_SOURCE_FILES} ${LINALG_HEADER_FILES}  )

add_executable( linalg_testrunner ${LINALG_TEST_SOURCE_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( linalg_testrunner linalg )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( linalg )
INSTALL_TEST( linalg_testrunner )