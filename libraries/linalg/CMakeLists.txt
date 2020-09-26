project( linalg CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES(   LINALG_INCLUDE_DIRECTORIES
                    LINALG_HEADER_FILES
                    LINALG_LIBRARY_SOURCE_FILES
                    LINALG_TEST_SOURCE_FILES )

file( GLOB LINALG_SOURCE_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
list( INSERT LINALG_TEST_SOURCE_FILES 0 ${LINALG_SOURCE_DRIVERS} )

include_directories( ${LINALG_INCLUDE_DIRECTORIES} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY(  linalg ${LINALG_LIBRARY_SOURCE_FILES} ${LINALG_HEADER_FILES}  )
TARGET_LINK_LIBRARIES_INSTALL( linalg cieutils )

add_executable( linalg_testrunner ${LINALG_TEST_SOURCE_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( linalg_testrunner linalg )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( linalg )
INSTALL_TEST( linalg_testrunner )