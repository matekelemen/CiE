# Collect package files recursively
# Expects the following directory layout:
# - package_name
#       - inc
#       - impl
#       - src
#       - test
#       - [packages] (optional, with identical substructure)
MACRO(COLLECT_PACKAGE directory headers sources tests)
    set( temp_headers "" )
    set( temp_impl "" )
    set( temp_sources "" )
    set( temp_tests "" )

    FILE( GLOB temp_headers ${directory}/inc/*.hpp )
    FILE( GLOB temp_impl ${directory}/impl/*.hpp )
    FILE( GLOB temp_sources ${directory}/src/*.cpp )
    FILE( GLOB temp_tests ${directory}/test/*.cpp )

    list( APPEND ${headers} ${temp_headers} )
    list( APPEND ${headers} ${temp_impl} )
    list( APPEND ${sources} ${temp_sources} )
    list( APPEND ${tests} ${temp_tests} )

    if( EXISTS "${directory}/packages" )
        SUBDIRLIST( packageNames "${directory}/packages" )
        foreach( packageName ${packageNames} )
            COLLECT_PACKAGE( "${directory}/packages/${packageName}"
                             packageHeaders
                             packageSources
                             packageTests )
            
            list( APPEND ${headers} ${packageHeaders} )
            list( APPEND ${sources} ${packageSources} )
            list( APPEND ${tests} ${packageTests} )
        endforeach()
    endif()
ENDMACRO()


# No recursive calls
MACRO( COLLECT_PACKAGES headers sources tests )
    set( headers "" )
    set( sources "" )
    set( tests "" )

    SUBDIRLIST( packageNames "${CMAKE_CURRENT_SOURCE_DIR}/packages" )

    foreach( packageName ${packageNames} )
        COLLECT_PACKAGE( "${CMAKE_CURRENT_SOURCE_DIR}/packages/${packageName}"
                         _headers _sources _tests )
        LIST( APPEND ${headers} ${_headers} )
        LIST( APPEND ${sources} ${_sources} )
        LIST( APPEND ${tests} ${_tests} )
    endforeach()
ENDMACRO()