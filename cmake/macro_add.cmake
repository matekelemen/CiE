include (GenerateExportHeader)
FUNCTION( ADD_SHARED_LIBRARY target )
    LIST( REMOVE_ITEM ARGV ${target} )
    ADD_LIBRARY( ${target} SHARED ${ARGV} )
    GENERATE_EXPORT_HEADER( ${target}
            BASE_NAME "${target}"
            EXPORT_MACRO_NAME "${target}_EXPORT"
            EXPORT_FILE_NAME "${target}_EXPORT.h"
            STATIC_DEFINE "${target}_BUILT_AS_STATIC" )

    target_include_directories( ${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc )
ENDFUNCTION( )