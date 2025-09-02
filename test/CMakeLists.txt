# paciFIST studios. 2025. MIT License

# NOTE: Check is compiled into an executable, so we actually need it as an include,
# rather than as just a library
include_directories(${CMAKE_SOURCE_DIR}/vendor/check/include)


# NOTE: same problem as noted in the src/CMakeList.  We have to add files manually
set(TEST_LIB_SOURCE_FILES
        # In order to register a test with check, you have to add that in
        # the unit_test_runner_engine.c file.
        ${CMAKE_CURRENT_SOURCE_DIR}/unit_test_runner_engine.c

        # all of the directories under the test directory, should have "tests" at the end
        # yes, it is redundant information, but we don't want any confusion when comparing
        # /src/memory to /test/memory --- especially if we're using this path generator

        # core tests
        ${CMAKE_CURRENT_SOURCE_DIR}/core_tests/error.test.c

        # datetime tests
        ${CMAKE_CURRENT_SOURCE_DIR}/datetime_test/datetime.test.c

        # engine tests
        ${CMAKE_CURRENT_SOURCE_DIR}/engine_tests/PFEngineState.test.c

        # log tests
        ${CMAKE_CURRENT_SOURCE_DIR}/log_tests/log.test.c

        # memory tests
        ${CMAKE_CURRENT_SOURCE_DIR}/algo_tests/search.test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/bit/bitfield.test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory_tests/RedBlackTree.test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory_tests/HashTable.test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory_tests/LinkedList.test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory_tests/PFCompactHashTable.test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory_tests/PFMemoryArena.test.c

        # os tests
        ${CMAKE_CURRENT_SOURCE_DIR}/os_tests/os_utility.test.c

        # string tests
        ${CMAKE_CURRENT_SOURCE_DIR}/string_tests/PString.test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string_tests/PFStringInternmentSingleton.test.c

        # type_tests
        ${CMAKE_CURRENT_SOURCE_DIR}/type_tests/TypedPtr.test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory_tests/PFAllocator.test.c
        ${CMAKE_CURRENT_SOURCE_DIR}/pftest_utilities.h


        # game projects
        ${CMAKE_CURRENT_SOURCE_DIR}/_game/slain_tests/SlainGameMode.test.c
        resource_tests/PFManagedResource.test.c
        resource_tests/PFResourceManager.test.c
)

# the run_unit_tests_engine is an executable that you could just run from the
# command-line.  When run, it just runs all of the unit tests, and reports the
# results to stdout/stderr
add_executable(run_unit_tests_engine ${TEST_LIB_SOURCE_FILES})
target_link_libraries(run_unit_tests_engine PRIVATE 
        ${PROJECT_NAME}_engine_lib
        check
        zlog
)

# this adds a some kind of command to CMake, which we can call
# form the primary CMakeLists file
add_test(NAME unit_tests COMMAND run_unit_tests_engine)