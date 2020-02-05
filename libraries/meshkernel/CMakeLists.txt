project( meshkernel CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
include_directories( inc )

file( GLOB MESHKERNEL_SOURCE_FILES src/*.cpp)
file( GLOB MESHKERNEL_TEST_FILES test/*.cpp)
file( GLOB MESHKERNEL_HEADER_FILES inc/*.h* )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY( meshkernel ${MESHKERNEL_SOURCE_FILES} ${MESHKERNEL_HEADER_FILES} ) 

add_executable( meshkernel_testrunner ${MESHKERNEL_TEST_FILES} ${MESHKERNEL_HEADER_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( meshkernel_testrunner meshkernel )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( meshkernel )
INSTALL_TEST( meshkernel_testrunner )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )
