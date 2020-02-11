project( glad C )

include_directories( include )

file( GLOB GLAD_HEADERS include/*.h* )
file( GLOB GLAD_SOURCES src/*.c* )

ADD_SHARED_LIBRARY( glad ${GLAD_HEADERS} ${GLAD_SOURCES} )
target_include_directories( glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include )