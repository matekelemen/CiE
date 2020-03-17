project( FEM CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/inc" )

file( GLOB FEM_SOURCE_FILES src/*.cpp)
file( GLOB FEM_TEST_FILES test/*.cpp)
file( GLOB FEM_HEADER_FILES inc/*.h* )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY( fem SHARED ${FEM_SOURCE_FILES} ${FEM_HEADER_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( fem )

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