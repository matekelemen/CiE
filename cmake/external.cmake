message( STATUS "\n---------- CIE EXTERNAL DEPENDENCIES BEGIN ----------" )

set( CIE_EXTERNAL_SOURCE_DIR "${CIE_SOURCE_DIR}/external" )

# Testing
include_directories( "${CIE_EXTERNAL_SOURCE_DIR}/catch" )

# Linear Algebra (eigen)
add_subdirectory( "${CIE_EXTERNAL_SOURCE_DIR}/eigen" )

# Bindings
set(PYBIND11_CPP_STANDARD "-std=c++${CMAKE_CXX_STANDARD}")
message( STATUS ${PYBIND11_CPP_STAMDARD} )
add_subdirectory( "${CIE_EXTERNAL_SOURCE_DIR}/pybind11" )

# Graphics
find_package( OpenGL REQUIRED )
add_subdirectory( "${CIE_EXTERNAL_SOURCE_DIR}/glfw" )
add_subdirectory( "${CIE_EXTERNAL_SOURCE_DIR}/glm" )
add_subdirectory( "${CIE_EXTERNAL_SOURCE_DIR}/glad" )
set( BUILD_SHARED_LIBS ON "Always build shared libraries" CACHE BOOL "Always build shared libraries" FORCE )

# FileIO
add_subdirectory( "${CIE_EXTERNAL_SOURCE_DIR}/xml" )
include_directories( "${CIE_EXTERNAL_SOURCE_DIR}/xml/src" )

message( STATUS "---------- CIE EXTERNAL DEPENDENCIES END ----------\n" )