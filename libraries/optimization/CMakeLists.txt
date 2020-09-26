project( optimization CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES(   OPTIMIZATION_INCLUDE_DIRECTORIES
                    OPTIMIZATION_HEADER_FILES
                    OPTIMIZATION_LIBRARY_SOURCE_FILES
                    OPTIMIZATION_TEST_SOURCE_FILES )

file( GLOB OPTIMIZATION_SOURCE_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
list( INSERT OPTIMIZATION_TEST_SOURCE_FILES 0 ${OPTIMIZATION_SOURCE_DRIVERS} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
# Add library from the collected source files. The headers are given so visual studio displays them
ADD_SHARED_LIBRARY( optimization ${OPTIMIZATION_LIBRARY_SOURCE_FILES} ${OPTIMIZATION_HEADER_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( optimization linalg )

# Add an executable and link to the library
add_executable( optimization_testrunner ${OPTIMIZATION_TEST_SOURCE_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( optimization_testrunner optimization linalg )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( optimization )
INSTALL_TEST( optimization_testrunner )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )
