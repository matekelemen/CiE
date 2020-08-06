project( FEM CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES(   FEM_INCLUDE_DIRECTORIES
                    FEM_HEADER_FILES
                    FEM_SOURCE_FILES
                    FEM_TEST_FILES )

include_directories( FEM_INCLUDE_DIRECTORIES )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../cieutils/inc" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/../linalg/inc" )
FILE( GLOB FEM_TEST_DRIVER ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
LIST( INSERT FEM_TEST_FILES 0 ${FEM_TEST_DRIVER} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY( fem SHARED ${FEM_SOURCE_FILES} ${FEM_HEADER_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( fem cieutils linalg )

add_executable( fem_testrunner ${FEM_TEST_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( fem_testrunner fem )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( fem )
INSTALL_TEST( fem_testrunner )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )