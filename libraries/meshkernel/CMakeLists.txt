project( meshkernel CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES( HEADERS
                  SOURCES
                  TEST_SOURCES )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY( meshkernel ${SOURCES} )
TARGET_LINK_LIBRARIES_INSTALL( meshkernel cieutils csg )

add_executable( meshkernel_testrunner ${TEST_SOURCES} )
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
