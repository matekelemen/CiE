message( STATUS "\n---------- CIE APPLICATIONS BEGIN ----------" )

foreach( applicationName ${CIE_APPLICATION_NAMES} )
    if( CIE_BUILD_APPLICATION_${applicationName} )
        add_subdirectory( "${applicationName}" )
    endif()
endforeach()

message( STATUS "---------- CIE APPLICATIONS END ----------\n" )