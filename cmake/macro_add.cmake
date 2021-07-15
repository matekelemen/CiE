include( GenerateExportHeader )
include( macro_install.cmake )

function( ADD_SHARED_LIBRARY target )

    # Remove target from argument list -> only link targets remain
    list( REMOVE_ITEM ARGV ${target} )

    message( STATUS "build shared library: ${target}" )
    add_library( ${target} SHARED ${ARGV} )

    # Include target export includes
    target_include_directories( ${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc" )

endfunction( )

function( ADD_TEST_EXECUTABLE target )

    # Remove target from argument list -> only link targets remain
    list( REMOVE_ITEM ARGV ${target} )

    message( STATUS "build test executable: ${target}" )
    add_executable( ${target} ${ARGV} )

    INSTALL_TEST( ${target} )

endfunction()