project( cieutils CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
COLLECT_PACKAGES( HEADERS
                  SOURCES
                  TESTS )
file( GLOB CIEUTILS_TEST_DRIVERS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
list( APPEND TESTS ${CIEUTILS_TEST_DRIVERS} )

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
# Add library from the collected source files. The headers are given so visual studio displays them
ADD_SHARED_LIBRARY( cieutils ${SOURCES} ${HEADERS} )
TARGET_LINK_LIBRARIES_INSTALL( cieutils )


# Add an executable and link to the library
ADD_TEST_EXECUTABLE( cieutils_testrunner ${TESTS} )
TARGET_LINK_LIBRARIES_INSTALL( cieutils_testrunner cieutils )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( cieutils )
install( FILES "${CMAKE_BINARY_DIR}/generated/cmake_variables.hpp" DESTINATION "${INSTALL_LIBRARY_PREFIX}" )