# Create install directory
install( DIRECTORY DESTINATION "${CMAKE_INSTALL_PREFIX}" )

# Headers
set( INSTALL_HEADER_PREFIX "${CMAKE_INSTALL_PREFIX}/include" )
install( DIRECTORY DESTINATION "${INSTALL_HEADER_PREFIX}" )

# Executable applications
if( MSVC )
    set( INSTALL_APPLICATION_PREFIX ${EXECUTABLE_INSTALL_PATH} )
else()
    set( INSTALL_APPLICATION_PREFIX "${CMAKE_INSTALL_PREFIX}/applications" )
endif()
install( DIRECTORY DESTINATION "${INSTALL_APPLICATION_PREFIX}" )

# Dynamic libraries
if( MSVC )
    set( INSTALL_LIBRARY_PREFIX ${LIBRARY_INSTALL_PATH} )
else()
    set( INSTALL_LIBRARY_PREFIX "${CMAKE_INSTALL_PREFIX}/lib" )
endif()
install( DIRECTORY DESTINATION "${INSTALL_LIBRARY_PREFIX}" )

# Executable tests
if( MSVC )
    set( INSTALL_TEST_PREFIX ${EXECUTABLE_INSTALL_PATH} )
else()
    set( INSTALL_TEST_PREFIX "${CMAKE_INSTALL_PREFIX}/tests" )
endif()
install( DIRECTORY DESTINATION "${INSTALL_TEST_PREFIX}" )

# Data
set( INSTALL_DATA_PREFIX "${CMAKE_INSTALL_PREFIX}/data" )
install( DIRECTORY DESTINATION "${INSTALL_DATA_PREFIX}" )

# Python bindings
if( MSVC )
    set( INSTALL_PYTHON_MODULES_PREFIX ${EXECUTABLE_INSTALL_PATH} )
else()
    set( INSTALL_PYTHON_MODULES_PREFIX "${CMAKE_INSTALL_PREFIX}/modules" )
endif()
install( DIRECTORY DESTINATION "${INSTALL_PYTHON_MODULES_PREFIX}" )

# Python scripts
set( INSTALL_PYTHON_SCRIPTS_PREFIX "${CMAKE_INSTALL_PREFIX}/scripts" )
install( DIRECTORY DESTINATION "${INSTALL_PYTHON_SCRIPTS_PREFIX}" )

# Output
set( CIE_INSTALL_OUTPUT_PREFIX "${CMAKE_INSTALL_PREFIX}/output" CACHE STRING "output path" )
set( INSTALL_DEBUG_OUTPUT_PREFIX "${CIE_INSTALL_OUTPUT_PREFIX}/debug" )
set( INSTALL_TEST_OUTPUT_PREFIX "${CIE_INSTALL_OUTPUT_PREFIX}/test" )
install( DIRECTORY DESTINATION "${CIE_INSTALL_OUTPUT_PREFIX}" )
install( DIRECTORY DESTINATION "${INSTALL_DEBUG_OUTPUT_PREFIX}" )
install( DIRECTORY DESTINATION "${INSTALL_TEST_OUTPUT_PREFIX}" )