set( test_dir ${VAR} )
message( STATUS "Run all tests in ${test_dir}" )
file( GLOB TEST_EXECUTABLES ${test_dir}/*_testrunner* )

foreach( test_executable ${TEST_EXECUTABLES} )
    message( STATUS "running ${test_executable}" )
    execute_process(    COMMAND "${test_executable}"
                        ENCODING AUTO )
endforeach()
message( STATUS "-----------------------------" )