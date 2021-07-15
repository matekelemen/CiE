project( FEM CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES( HEADERS
                  SOURCES
                  TEST_SOURCES )

FILE( GLOB TEST_DRIVER ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
LIST( INSERT TEST_SOURCES 0 ${TEST_DRIVER} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY( fem SHARED ${SOURCES} )
target_include_directories( fem PUBLIC "${CIE_EXTERNAL_SOURCE_DIR}/eigen" )
TARGET_LINK_LIBRARIES_INSTALL( fem cieutils linalg )
INSTALL_LIBRARY( fem )

ADD_TEST_EXECUTABLE( fem_testrunner ${TEST_SOURCES} )
TARGET_LINK_LIBRARIES_INSTALL( fem_testrunner fem )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )