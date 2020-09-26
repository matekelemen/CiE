MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY "${curdir}/${child}" )
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()


# Collect package files
# Expects the following directory layout:
# - package_name
#       - inc
#       - impl
#       - src
#       - test
MACRO(COLLECT_PACKAGE directory headers sources tests)
    FILE( GLOB temp_headers ${directory}/inc/*.hpp )
    FILE( GLOB temp_impl ${directory}/impl/*.hpp )
    FILE( GLOB temp_sources ${directory}/src/*.cpp )
    FILE( GLOB temp_tests ${directory}/test/*.cpp )

    SET( ${headers} ${temp_headers} )
    LIST( APPEND headers ${temp_impl} )
    SET( ${sources} ${temp_sources} )
    SET( ${tests} ${temp_tests} )
ENDMACRO()


MACRO( COLLECT_PACKAGES headers sources tests )
    SUBDIRLIST( packageNames "${CMAKE_CURRENT_SOURCE_DIR}/packages" )
    foreach( packageName ${packageNames} )
        COLLECT_PACKAGE(    "${CMAKE_CURRENT_SOURCE_DIR}/packages/${packageName}"
                            _headers _sources _tests )
        LIST( APPEND ${headers} ${_headers} )
        LIST( APPEND ${sources} ${_sources} )
        LIST( APPEND ${tests} ${_tests} )
    endforeach()
ENDMACRO()