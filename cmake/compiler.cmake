# If compiler is g++: Enable further  warnings and treat all warnings as errors. fPIC stands for position independent code.
if( CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Werror -fPIC -Wreturn-type" )
elseif( MSVC )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
endif( CMAKE_COMPILER_IS_GNUCXX )