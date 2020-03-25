project( glad C )

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )

file( GLOB GLAD_HEADERS inc/*.h* )
file( GLOB GLAD_SOURCES src/*.c* )

ADD_SHARED_LIBRARY( glad ${GLAD_HEADERS} ${GLAD_SOURCES} )
target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)

INSTALL_LIBRARY( glad )