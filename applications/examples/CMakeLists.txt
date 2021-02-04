project( examples CXX )

# Define header path
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/inc" )

# Collect headers and sources
file( GLOB HEADERS inc/*.h* )
file( GLOB SOURCES src/*.cpp )

FUNCTION( ADD_DRIVER driver )
    LIST( REMOVE_ITEM ARGV ${driver} )

    get_filename_component( executable_name ${driver} NAME_WE )
    message( STATUS "Add executable: ${executable_name}" )

    add_executable( ${executable_name} ${HEADERS} ${SOURCES} ${driver} )
    TARGET_LINK_LIBRARIES_INSTALL( ${executable_name} ${ARGV} )

    INSTALL_APPLICATION_EXECUTABLE( ${executable_name} )
ENDFUNCTION()

# Build executable
ADD_DRIVER( "drivers/marching_squares.cpp" ciegl )
ADD_DRIVER( "drivers/marching_squares_ellipsis.cpp" ciegl )
ADD_DRIVER( "drivers/fcm_ellipsoid.cpp" csg )

# Copy python scripts
#file( GLOB PYTHON_SCRIPTS scripts/*.py )
#INSTALL_APPLICATION_SCRIPTS( ${PYTHON_SCRIPTS} )

# Copy data
#INSTALL_APPLICATION_DATA( )