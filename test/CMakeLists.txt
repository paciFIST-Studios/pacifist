# paciFIST studios. 2025. MIT License

# NOTE: Check is compiled into an executable, so we actually need it as an include,
# rathr than as just a library
include_directories(${CMAKE_SOURCE_DIR}/vendor/check/include)

# NOTE: same problem as noted in the src/CMakeList.  We have to add files manually
set(TEST_LIB_SOURCE_FILES
        # In order to register a test with check, you have to add that in
        # the unit_test_runner_engine.c file.
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test_runner_engine.c
       
        # all of the directories under the test directory, should have "tests" at the end
        # yes, it is redundant information, but we don't want any confusion when comparing
        # /src/memory to /test/memory --- especially if we're using this path generator
        
        # memory_tests
        ${CMAKE_CURRENT_SOURCE_DIR}/memory_tests/RedBlackTree.test.c
        
)

# the run_unit_tests_engine is an executable that you could just run from the
# command-line.  When run, it just runs all of the unit tests, and reports the
# results to stdout/stderr
add_executable(run_unit_tests_engine ${TEST_LIB_SOURCE_FILES})
target_link_libraries(run_unit_tests_engine PRIVATE 
        ${PROJECT_NAME}_engine_lib
       check 
)

# this adds a some kind of command to CMake, which we can call
# form the primary CMakeLists file
add_test(NAME unit_tests COMMAND run_unit_tests_engine)