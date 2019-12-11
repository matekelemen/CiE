SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR} )
foreach( subdirectory ${subdirs} )
    ADDFILES( ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory} )
endforeach()