# Testing
include_directories( "${CIE_SOURCE_DIR}/external/catch" )

# Bindings
set(PYBIND11_CPP_STANDARD -std=c++17)
add_subdirectory( "${CIE_SOURCE_DIR}/external/pybind11" )

# Graphics
find_package( OpenGL REQUIRED )
add_subdirectory( "${CIE_SOURCE_DIR}/external/glfw" )
add_subdirectory( "${CIE_SOURCE_DIR}/external/glm" )
add_subdirectory( "${CIE_SOURCE_DIR}/external/glad" )
set( BUILD_SHARED_LIBS ON "Always build shared libraries" CACHE BOOL "Always build shared libraries" FORCE )

# FileIO
add_subdirectory( "${CIE_SOURCE_DIR}/external/xml" )
include_directories( "${CIE_SOURCE_DIR}/external/xml/src" )