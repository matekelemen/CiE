project( benchmarks CXX )

# Define header path
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/inc" )

# Collect headers and sources
file( GLOB HEADERS inc/*.h* )
file( GLOB SOURCES src/*.cpp )

# Build executables

message( STATUS "Add executable: thread_pool_benchmark" )
add_executable( thread_pool_benchmark ${HEADERS} ${SOURCES} "drivers/thread_pool_benchmark.cpp" )
TARGET_LINK_LIBRARIES_INSTALL( thread_pool_benchmark cieutils )
INSTALL_APPLICATION_EXECUTABLE( thread_pool_benchmark )

message( STATUS "Add executable: space_tree_benchmark" )
add_executable( space_tree_benchmark ${HEADERS} ${SOURCES} "drivers/space_tree_benchmark.cpp" )
TARGET_LINK_LIBRARIES_INSTALL( space_tree_benchmark csg )
INSTALL_APPLICATION_EXECUTABLE( space_tree_benchmark )

# Copy data
#INSTALL_APPLICATION_DATA( )