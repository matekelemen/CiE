# ---------------------------------------------------------
# GENERAL OPTIONS
# ---------------------------------------------------------
# What to install
option( CIE_INSTALL_TESTS "Install unit-and system tests" ON )

# ---------------------------------------------------------
# PARALLELIZATION
# ---------------------------------------------------------
set( CIE_ENABLE_OPENMP ON CACHE BOOL "enable openmp directives" )
set( CIE_ENABLE_OPENACC OFF CACHE BOOL "enable openacc directives" )

# ---------------------------------------------------------
# OPENGL OPTIONS
# ---------------------------------------------------------
option( CIE_USE_CPP_GRAPHICS "Compile OpenGL and GLEW" ON )
set( OpenGL_GL_PREFERENCE "GLVND" CACHE STRING "GLVND or LEGACY" )

# ---------------------------------------------------------
# MACRO OPTIONS
# ---------------------------------------------------------
set( CIE_ENABLE_OUT_OF_RANGE_TESTS ON CACHE BOOL "" )
if( ${CIE_ENABLE_OUT_OF_RANGE_TESTS} )
    add_compile_definitions( CIE_ENABLE_OUT_OF_RANGE_TESTS )
endif()