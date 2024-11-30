add_executable(run_tests
        run_tests.cc
        json_key_path_tests.cc
        json_object_tests.cc
)

target_link_libraries(run_tests
        gtest
        gtest_main
        boost_json
        dkjsonobject
)
add_test(NAME run_tests COMMAND run_tests)

