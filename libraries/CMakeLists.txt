message( STATUS "\n---------- CIE LIBRARIES BEGIN ----------" )

foreach( libraryName ${CIE_LIBRARY_NAMES} )
    if( CIE_BUILD_LIBRARY_${libraryName} )
        add_subdirectory( "${libraryName}" )
    endif()
endforeach()

#add_subdirectory( linalg )
#add_subdirectory( splinekernel )
#add_subdirectory( CSG )
#add_subdirectory( meshkernel )
#add_subdirectory( FEM )
#add_subdirectory( ciegl )

message( STATUS "---------- CIE LIBRARIES END ----------\n" )