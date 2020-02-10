FUNCTION( TARGET_LINK_PYTHON_BINDINGS target )
  # Remove target from argument list
  LIST( REMOVE_ITEM ARGV ${target} )
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
            $<TARGET_FILE_DIR:${library}>/${library}.dll
            ${INSTALL_PYTHON_MODULES_PREFIX}/py${projectName})
    endforeach(  )
  endif()

  set_target_properties( ${target} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDFUNCTION()


FUNCTION( TARGET_LINK_LIBRARIES_INSTALL target )
  # Remove target from argument list
  LIST( REMOVE_ITEM ARGV ${target} )
  # Stackoverflow magic
  set_target_properties( ${target} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE )
  target_link_libraries( ${target} ${ARGV} )
  
  if(UNIX AND NOT APPLE)
    set_target_properties( ${target} PROPERTIES INSTALL_RPATH ${INSTALL_LIBRARY_PREFIX} )
  endif()
  
  set_target_properties( ${target} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDFUNCTION()


MACRO( TARGET_LINK_GRAPHICS_INSTALL target )
  TARGET_LINK_LIBRARIES_INSTALL( ${target} glfw OpenGL::GL glm )
ENDMACRO()