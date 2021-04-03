# Build configuration
if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel" FORCE )
endif()

message( STATUS "Compile CiE in ${CMAKE_BUILD_TYPE} mode" )

# -------------------------------------------------
# GCC
# -------------------------------------------------
if ( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )

	# Compiler definition in preprocessor
	add_compile_definitions( GCC )
	
	# Warnings and errors
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -fPIC -Wreturn-type" )
    #set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )
	
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