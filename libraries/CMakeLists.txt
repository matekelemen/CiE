# Collect subdirectories
SUBDIRLIST( subdirs ${CMAKE_CURRENT_SOURCE_DIR} )

# Add subdirectories
foreach( subdir ${subdirs} )
    add_subdirectory( ${subdir} )
endforeach()

#add_subdirectory( linalg )
#add_subdirectory( splinekernel )
#add_subdirectory( CSG )
#add_subdirectory( meshkernel )
#add_subdirectory( optimization )
#add_subdirectory( FEM )
#add_subdirectory( ciegl )