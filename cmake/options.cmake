# ---------------------------------------------------------
# GENERAL OPTIONS
# ---------------------------------------------------------
# Optional tests
option( CIE_BUILD_TESTS "Build unit-and system tests" ON )

# ---------------------------------------------------------
# CONCURRENCY
# ---------------------------------------------------------
if( MSVC )
    set( CIE_ENABLE_OPENMP OFF CACHE BOOL "enable openmp directives" )
else()
    set( CIE_ENABLE_OPENMP ON CACHE BOOL "enable openmp directives" )
endif()

# ---------------------------------------------------------
# OPENGL OPTIONS
# ---------------------------------------------------------
set( OpenGL_GL_PREFERENCE "GLVND" CACHE STRING "GLVND or LEGACY" )

# ---------------------------------------------------------
# MACRO OPTIONS
# ---------------------------------------------------------
set( CIE_ENABLE_EXCEPTION_TRACING ON CACHE BOOL "Trace thrown exceptions through functions" )
if ( ${CIE_ENABLE_EXCEPTION_TRACING} )
    add_compile_definitions( CIE_ENABLE_EXCEPTION_TRACING )
endif()

set( CIE_ENABLE_OUT_OF_RANGE_TESTS ON CACHE BOOL "" )
if( ${CIE_ENABLE_OUT_OF_RANGE_TESTS} )
    add_compile_definitions( CIE_ENABLE_OUT_OF_RANGE_TESTS )
endif()

set( CIE_ENABLE_DIVISION_BY_ZERO_CHECKS ON CACHE BOOL "" )
if( ${CIE_ENABLE_DIVISION_BY_ZERO_CHECKS} )
    add_compile_definitions( CIE_ENABLE_DIVISION_BY_ZERO_CHECKS )
endif()

set( CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS ON CACHE BOOL "" )
if( ${CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS} )
    add_compile_definitions( CIE_ENABLE_RUNTIME_GEOMETRY_CHECKS )
endif()

set( CIE_ENABLE_DEBUG_FILE_OUTPUT ON CACHE BOOL "Enable writing debug info to files" )
if ( ${CIE_ENABLE_DEBUG_FILE_OUTPUT} )
    add_compile_definitions( CIE_ENABLE_DEBUG_FILE_OUTPUT )
endif()