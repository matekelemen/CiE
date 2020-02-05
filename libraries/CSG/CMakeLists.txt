project( CSG CXX )

# ---------------------------------------------------------
# GATHER FILES AND INCLUDE DIRECTORIES
# ---------------------------------------------------------
include_directories( inc )
include_directories( ../linalg/inc )

# Initialize file lists
file( GLOB CSG_SOURCE_FILES src/*.cpp)
file( GLOB_RECURSE CSG_TEST_FILES test/*.cpp)
file( GLOB CSG_HEADER_FILES inc/*.h* )
file( GLOB CSG_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc )

# Define macro to add header and source files from subdirectories
MACRO( ADDFILES curdir )
    # Collect header and source files
    file( GLOB TEMP_HEADERS ${curdir}/inc/*.h* )
    file( GLOB TEMP_SOURCES ${curdir}/src/*.cpp )
    file( GLOB TEMP_TESTS ${curdir}/test/*.cpp )
    # Append file lists and set them in the parent scope
    list( APPEND CSG_INCLUDE_DIRECTORIES ${curdir}/inc )
    set( CSG_INCLUDE_DIRECTORIES ${CSG_INCLUDE_DIRECTORIES} PARENT_SCOPE )
    list( APPEND CSG_HEADER_FILES ${TEMP_HEADERS} )
    set( CSG_HEADER_FILES ${CSG_HEADER_FILES} PARENT_SCOPE )
    list( APPEND CSG_SOURCE_FILES ${TEMP_SOURCES} )
    set( CSG_SOURCE_FILES ${CSG_SOURCE_FILES} PARENT_SCOPE )
    list( APPEND CSG_TEST_FILES ${TEMP_TESTS} )
    set( CSG_TEST_FILES ${CSG_TEST_FILES} PARENT_SCOPE )
ENDMACRO()

SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR} )
foreach( subdirectory ${subdirs} )
    ADDFILES( ${subdirectory} )
endforeach()

# ---------------------------------------------------------
# LIBRARIES AND EXECUTABLES
# ---------------------------------------------------------
ADD_SHARED_LIBRARY( csg ${CSG_SOURCE_FILES} ${CSG_HEADER_FILES}  )
TARGET_LINK_LIBRARIES_INSTALL( csg linalg )

add_executable( csg_testrunner ${CSG_TEST_FILES} )
TARGET_LINK_LIBRARIES_INSTALL( csg_testrunner csg linalg )

# ---------------------------------------------------------
# INSTALL
# ---------------------------------------------------------
INSTALL_LIBRARY( csg )
INSTALL_TEST( csg_testrunner )

# ---------------------------------------------------------
# WINDOWS
# ---------------------------------------------------------
# To be able to debug in Windows we have to copy the desired library to the project debug folder.
# This adds a post build command that after building the executable copies the library over.
if( MSVC )
    add_custom_command(TARGET csg_testrunner POST_BUILD        	
        COMMAND ${CMAKE_COMMAND} -E copy 					
            $<TARGET_FILE_DIR:linalg>/linalg.dll  
            $<TARGET_FILE_DIR:csg_testrunner>)
endif( )

# ---------------------------------------------------------
# PYTHON
# ---------------------------------------------------------
add_subdirectory( python )