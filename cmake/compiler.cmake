# Build configuration
set( CIE_COMPILE_OPTIONS "debug" CACHE STRING "debug or release" )
message( STATUS "Compile CiE in ${CIE_COMPILE_OPTIONS} mode" )

# -------------------------------------------------
# GCC
# -------------------------------------------------
if ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )

	# Compiler definition in preprocessor
	add_compile_definitions( GCC )
	
	# Warnings and errors
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -fPIC -Wreturn-type" )
    #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )
	
	# Compiler optimizations
	if( "${CIE_COMPILE_OPTIONS}" STREQUAL "debug" )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG" )
	elseif( "${CIE_COMPILE_OPTIONS}" STREQUAL "release" )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s -march=native -mtune=native -DNDEBUG" )
	else()
		message( SEND_ERROR "Unrecognized compile option! Choose debug or release." )
	endif()
	
	# OpenMP
	if( ${CIE_ENABLE_OPENMP} )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp" )
	endif()
	
	# OpenACC
	if( ${CIE_ENABLE_OPENACC} )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenacc" )
	endif()

# -------------------------------------------------
# MSVC
# -------------------------------------------------
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )

	# Compiler definition in preprocessor
	add_compile_definitions( MSVC )
	
	# Global flags
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest" )
	
	# Compiler optimizations
	if( "${CIE_COMPILE_OPTIONS}" STREQUAL "debug" )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}" )
	elseif( "${CIE_COMPILE_OPTIONS}" STREQUAL "release" )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}" )
	else()
		message( SEND_ERROR "Unrecognized compile option! Choose debug or release." )
	endif()
	
	# OpenMP
	if( ${CIE_ENABLE_OPENMP} )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp" )
	endif()
	
# -------------------------------------------------
# CLANG
# -------------------------------------------------
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )

	# Compiler definition in preprocessor
	add_compile_definitions( CLANG )
	
	# TODO

endif()