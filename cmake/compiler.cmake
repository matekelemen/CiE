
if( CMAKE_COMPILER_IS_GNUCXX )

    # If compiler is g++: Enable further  warnings and treat all warnings as errors. fPIC stands for position independent code.
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror -fPIC -Wreturn-type" )

    # Compiler optimizations
    set( CIE_COMPILE_OPTIONS "debug" CACHE STRING "debug or release" )
    if( "${CIE_COMPILE_OPTIONS}" STREQUAL "debug" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG" )
    elseif( "${CIE_COMPILE_OPTIONS}" STREQUAL "release" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s -march=native -mtune=native -DNDEBUG" )
    else()
        message( SEND_ERROR "Unrecognized compile option! Choose debug or release." )
    endif()

    message( STATUS "Compile CiE in ${CIE_COMPILE_OPTIONS} mode" )

elseif( MSVC )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
endif( CMAKE_COMPILER_IS_GNUCXX )


# OpenMP
option( CIE_OPENMP ON )
if( ${CIE_OPENMP} )
    if( CMAKE_COMPILER_IS_GNUCXX )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp" )
    elseif( MSVC )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp" )
    endif()
endif()