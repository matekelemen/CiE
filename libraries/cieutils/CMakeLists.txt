project( cieutils CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES(   CIEUTILS_INCLUDE_DIRECTORIES
                    CIEUTILS_HEADER_FILES
                    CIEUTILS_SOURCE_FILES
                    CIEUTILS_TEST_FILES )
file( GLOB CIEUTILS_TEST_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
list( APPEND CIEUTILS_TEST_FILES ${CIEUTILS_TEST_DRIVERS} )

include_directories( "${CIEUTILS_INCLUDE_DIRECTORIES}" )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
# Add library from the collected source files. The headers are given so visual studio displays them
ADD_SHARED_LIBRARY( cieutils ${CIEUTILS_SOURCE_FILES} ${CIEUTILS_HEADER_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( cieutils )


# Add an executable and link to the library
add_executable( cieutils_testrunner ${CIEUTILS_TEST_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( cieutils_testrunner cieutils )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( cieutils )
INSTALL_TEST( cieutils_testrunner )
install( FILES "${CMAKE_BINARY_DIR}/generated/cmake_variables.hpp" DESTINATION "${INSTALL_LIBRARY_PREFIX}" )