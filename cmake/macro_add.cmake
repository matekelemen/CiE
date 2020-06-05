include (GenerateExportHeader)

function( ADD_SHARED_LIBRARY target )

    # Remove target from argument list -> only link targets remain
    list( REMOVE_ITEM ARGV ${target} )

    add_library( ${target} SHARED ${ARGV} )
    GENERATE_EXPORT_HEADER( ${target}
            BASE_NAME "${target}"
            EXPORT_MACRO_NAME "${target}_EXPORT"
            EXPORT_FILE_NAME "${target}_EXPORT.hpp"
            STATIC_DEFINE "${target}_BUILT_AS_STATIC" )

    # Include target export includes
    target_include_directories( ${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/inc" )

endfunction( )