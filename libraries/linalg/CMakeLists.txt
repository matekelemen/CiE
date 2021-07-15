project( linalg CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES( HEADERS
                  SOURCES
                  TESTS )

file( GLOB LINALG_SOURCE_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
list( INSERT TESTS 0 ${LINALG_SOURCE_DRIVERS} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY(  linalg ${SOURCES} ${HEADERS}  )
target_include_directories( linalg PUBLIC "${CIE_EXTERNAL_SOURCE_DIR}/eigen" )
TARGET_LINK_LIBRARIES_INSTALL( linalg cieutils )
INSTALL_LIBRARY( linalg )

ADD_TEST_EXECUTABLE( linalg_testrunner ${TESTS} )
TARGET_LINK_LIBRARIES_INSTALL( linalg_testrunner linalg )