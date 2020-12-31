include (GenerateExportHeader)

function( ADD_SHARED_LIBRARY target )

    # Remove target from argument list -> only link targets remain
    list( REMOVE_ITEM ARGV ${target} )

    add_library( ${target} SHARED ${ARGV} )

    # Include target export includes
    target_include_directories( ${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc" )

endfunction( )