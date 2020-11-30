MACRO( MAKE_INSTALL_DIR path )
    if ( NOT EXISTS ${path} )
        INSTALL( DIRECTORY DESTINATION ${path} )
    elseif( NOT IS_DIRECTORY ${path} )
        file( REMOVE_RECURSE ${path} )
        INSTALL( DIRECTORY DESTINATION ${path} )
    endif()
ENDMACRO()


MACRO( INSTALL_LIBRARY target )
  file( GLOB export_header ${CMAKE_CURRENT_BINARY_DIR}/*.hpp )

  if( MSVC )
    install( TARGETS ${target} RUNTIME DESTINATION ${INSTALL_LIBRARY_PREFIX} )
    install( FILES ${export_header} DESTINATION ${INSTALL_LIBRARY_PREFIX} )
  else( )
    install( TARGETS ${target} LIBRARY DESTINATION ${INSTALL_LIBRARY_PREFIX} )
    install( FILES ${export_header} DESTINATION ${INSTALL_LIBRARY_PREFIX} )
  endif( )
ENDMACRO()


MACRO( INSTALL_TEST target )
  if( CIE_INSTALL_TESTS )
    install( TARGETS ${target} RUNTIME DESTINATION ${INSTALL_TEST_PREFIX} )
  endif()
ENDMACRO()


MACRO( INSTALL_PYTHON_BINDINGS target moduleName )
    set( MODULE_INSTALL_DIR "${INSTALL_PYTHON_MODULES_PREFIX}/${moduleName}" )
    MAKE_INSTALL_DIR( "${MODULE_INSTALL_DIR}" )
    install( TARGETS ${target} LIBRARY DESTINATION "${MODULE_INSTALL_DIR}/" )
    
    ## Copy dependencies
    #get_target_property( LINKED_DEPENDENCIES ${target} LINK_LIBRARIES )
    #foreach( dependency ${LINKED_DEPENDENCIES} )
    #    add_custom_command(
    #        TARGET ${target} POST_BUILD
    #        COMMAND ${CMAKE_COMMAND} -E copy
    #        "$<TARGET_FILE_DIR:${dependency}>/${dependency}.dll"
    #        "${MODULE_INSTALL_DIR}/"
    #    )
    #endforeach()
ENDMACRO()


MACRO( INSTALL_PYTHON_MODULE moduleFolder )
  if( ${ARGC} EQUAL 1 )
    set( MODULE_INSTALL_DIR "${INSTALL_PYTHON_MODULES_PREFIX}" )
  elseif( ${ARGC} EQUAL 2 )
    set( moduleName ${ARGV1} )
    set( MODULE_INSTALL_DIR "${INSTALL_PYTHON_MODULES_PREFIX}/${moduleName}" )
  endif()
  
  MAKE_INSTALL_DIR( "${MODULE_INSTALL_DIR}" )
  install( DIRECTORY "${moduleFolder}" DESTINATION "${MODULE_INSTALL_DIR}" )
ENDMACRO()


MACRO( INSTALL_PYTHON_SCRIPTS scriptFolder )
  MAKE_INSTALL_DIR( "${INSTALL_PYTHON_SCRIPTS_PREFIX}/${PROJECT_NAME}" )
  install( DIRECTORY "${scriptFolder}/" DESTINATION "${INSTALL_PYTHON_SCRIPTS_PREFIX}/${PROJECT_NAME}/" )
ENDMACRO()


MACRO( INSTALL_APPLICATION_EXECUTABLE target )
    if( MSVC )
        install( TARGETS ${target} RUNTIME DESTINATION "${EXECUTABLE_INSTALL_PATH}/" )
        # Copy dependencies
        get_target_property( LINKED_DEPENDENCIES ${target} LINK_LIBRARIES )
        foreach( dependency ${LINKED_DEPENDENCIES} )
            add_custom_command(
                TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE_DIR:${dependency}>/${dependency}.dll"
                "${EXECUTABLE_INSTALL_PATH}/${dependency}.dll"
            )
        endforeach()
    else()
        MAKE_INSTALL_DIR( "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}" )
        install( TARGETS ${target} RUNTIME DESTINATION "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}" )
    endif()
ENDMACRO()


MACRO( INSTALL_APPLICATION_LIBRARY target )
    install( DIRECTORY DESTINATION "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}" )
    if( MSVC )
        install( TARGETS ${target} RUNTIME DESTINATION "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}/" )
    else()
        install( TARGETS ${target} LIBRARY DESTINATION "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}" )
    endif()
ENDMACRO()


MACRO( INSTALL_APPLICATION_PYTHON_BINDINGS target )
    MAKE_INSTALL_DIR( "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}" )
    install( TARGETS ${target} LIBRARY DESTINATION "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}" )
ENDMACRO()


MACRO( INSTALL_APPLICATION_SCRIPTS )
    #install( DIRECTORY DESTINATION ${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}/scripts )
    #install( FILES ${ARGV} DESTINATION ${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}/scripts )
    MAKE_INSTALL_DIR( "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}" )
    install( FILES ${ARGV} DESTINATION "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}" )
ENDMACRO()


MACRO( INSTALL_APPLICATION_DATA )
    MAKE_INSTALL_DIR( "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}/data" )
    install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/" DESTINATION "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}/data" )
    #install( FILES ${ARGV} DESTINATION "${INSTALL_APPLICATION_PREFIX}/${PROJECT_NAME}/data" )
ENDMACRO()


MACRO( INSTALL_DATA dataFolder )
  install( DIRECTORY "${dataFolder}/" DESTINATION "${INSTALL_DATA_PREFIX}/${PROJECT_NAME}/" )
ENDMACRO()