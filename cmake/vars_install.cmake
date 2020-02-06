install( DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Executable applications
set( INSTALL_APPLICATION_PREFIX ${CMAKE_INSTALL_PREFIX}/applications )
install( DIRECTORY "applications" DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Dynamic libraries
set( INSTALL_LIBRARY_PREFIX ${CMAKE_INSTALL_PREFIX}/libraries )
install( DIRECTORY "libraries" DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Executable tests
set( INSTALL_TEST_PREFIX ${CMAKE_INSTALL_PREFIX}/tests )
install( DIRECTORY "tests" DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Data
set( INSTALL_DATA_PREFIX ${CMAKE_INSTALL_PREFIX}/data )
install( DIRECTORY "data" DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Python bindings
set( INSTALL_PYTHON_MODULES_PREFIX ${CMAKE_INSTALL_PREFIX}/modules )
install( DIRECTORY "modules" DESTINATION ${CMAKE_INSTALL_PREFIX} )

# Python scripts
set( INSTALL_PYTHON_SCRIPTS_PREFIX ${CMAKE_INSTALL_PREFIX}/scripts )
install( DIRECTORY "scripts" DESTINATION ${CMAKE_INSTALL_PREFIX} )