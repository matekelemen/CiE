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
INSTALL_LIBRARY( meshkernel )

ADD_TEST_EXECUTABLE( meshkernel_testrunner ${TEST_SOURCES} )
TARGET_LINK_LIBRARIES_INSTALL( meshkernel_testrunner meshkernel )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )
