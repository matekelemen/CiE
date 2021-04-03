FUNCTION( CIE_CHECK_INTERNAL_DEPENDENCIES target dependencies )
    set( unmet_dependencies "" )

    foreach( dependency ${dependencies} )
    if( DEFINED CIE_BUILD_LIBRARY_${dependency} )
            if( NOT CIE_BUILD_LIBRARY_${dependency} )
                list( APPEND unmet_dependencies "${dependency}" )
            endif()
        endif()
    endforeach()

    if( unmet_dependencies )
        message( FATAL_ERROR "Target ${target} has unmet dependencies: ${unmet_dependencies}" )
    endif()
ENDFUNCTION()


FUNCTION( TARGET_LINK_PYTHON_BINDINGS target )
    # Remove target from argument list
    LIST( REMOVE_ITEM ARGV ${target} )

    # Check dependencies
    if( ARGV )
        CIE_CHECK_INTERNAL_DEPENDENCIES( ${target} ${ARGV} )
    endif()

    # Stackoverflow magic
    set_target_properties( ${target} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE )
    target_link_libraries( ${target} PRIVATE ${ARGV} )
    
    if(UNIX AND NOT APPLE)
        set_target_properties( ${target} PROPERTIES INSTALL_RPATH ${INSTALL_LIBRARY_PREFIX} )
    elseif( WIN32 )
        STRING( TOLOWER ${PROJECT_NAME} projectName )
        foreach( library ${ARGV} )
            add_custom_command(TARGET ${target} POST_BUILD        	
            COMMAND ${CMAKE_COMMAND} -E copy 					
                "$<TARGET_FILE_DIR:${library}>/${library}.dll"
                "${INSTALL_PYTHON_MODULES_PREFIX}/py${projectName}" )
        endforeach(  )
    endif()

    set_target_properties( ${target} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDFUNCTION()


FUNCTION( TARGET_LINK_LIBRARIES_INSTALL target )
    # Remove target from argument list
    LIST( REMOVE_ITEM ARGV ${target} )

    # Check dependencies
    if( ARGV )
        CIE_CHECK_INTERNAL_DEPENDENCIES( ${target} ${ARGV} )
    endif()

    # Stackoverflow magic
    #set_target_properties( ${target} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE )
    target_link_libraries( ${target} PUBLIC ${ARGV} )
  
    if(UNIX AND NOT APPLE)
      set_target_properties( ${target} PROPERTIES INSTALL_RPATH ${INSTALL_LIBRARY_PREFIX} )
    endif()
  
    set_target_properties( ${target} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

ENDFUNCTION()


MACRO( TARGET_LINK_GRAPHICS_INSTALL target )
    TARGET_LINK_LIBRARIES_INSTALL( ${target} glad glfw OpenGL::GL glm )
ENDMACRO()