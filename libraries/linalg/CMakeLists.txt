project( linalg CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES(   LINALG_HEADER_FILES
                    LINALG_LIBRARY_SOURCE_FILES
                    LINALG_TEST_SOURCE_FILES )

file( GLOB LINALG_SOURCE_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
list( INSERT LINALG_TEST_SOURCE_FILES 0 ${LINALG_SOURCE_DRIVERS} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY(  linalg ${LINALG_LIBRARY_SOURCE_FILES} ${LINALG_HEADER_FILES}  )
target_include_directories( linalg PUBLIC "${CIE_EXTERNAL_SOURCE_DIR}/eigen" )
TARGET_LINK_LIBRARIES_INSTALL( linalg cieutils )

add_executable( linalg_testrunner ${LINALG_TEST_SOURCE_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( linalg_testrunner linalg )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( linalg )
INSTALL_TEST( linalg_testrunner )