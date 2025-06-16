// paciFIST studios. 2025. MIT License

#ifndef UNIT_TEST_RUNNER_H
#define UNIT_TEST_RUNNER_H

#include <check.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// START_TEST and END_TEST are part of the check system
START_TEST(unit_test_system_check) {
    // historically, this has given errors if you add a message at the end
    fail_unless(1 == 1);
}END_TEST


// NOTE: these variables exist to streamline the process of registering a test
// You should NOT use them manually.  Use the macros to register tests.  If
// you still don't want to, see tutorial.test.c for an example
TCase* global_test_handle;
Suite* global_suite;
SRunner* global_runner;
char * global_module;

// param: char*          module // name of suite
#define BUILD_SUITE(module) do {                        \
    global_module = module;                             \
    global_suite = suite_create(global_module);         \
    global_runner = srunner_create(global_suite);       \
} while (0);                                            \

// param: fn*    fn // fn name, defined using <check.h> macros
#define ADD_TEST(fn) do {                               \
    global_test_handle = tcase_create(global_module);   \
    suite_add_tcase(global_suite, global_test_handle);  \
    tcase_add_test(global_test_handle, fn);             \
} while(0);                                             \

// param: int     fail_count
#define RUN_SUITE(fail_count) do {                      \
    srunner_run_all(global_runner, CK_ENV);             \
    fail_count = srunner_ntests_failed(global_runner);  \
    srunner_free(global_runner);                        \
    global_module = NULL;                               \
} while(0);                                             \


/** --------------------------------------------------------------------------------------------------------------------
 *  Ellie's Note:
 *
 *      There are some concessions we have to make in order to simplify
 *      the process of adding tests.  Here's what to do:
 *
 *  1. Start a test suite by using the BUILD_SUITE macro
 *  2. Add tests using the ADD_TEST macro
 *      a. you can only add tests to the suite you just built
 *      b. you can only add tests, which are declared suing the <check.h> macros (ie: START_TEST)
 *  3. Run the test suite using the RUN_SUITE macro
 *
 *  You have to do all 3.
 *  Tou have to do them in order.
 *  You can't stop mid-process.
 *
 *  By using these macros, you agree to the above stipulations
 * 
 *----------------------------------------------------------------------------------------------------------------------
 */

// Test Includes: Manually include your code here.  (keep it in alphabetical order)
#include "../test/memory_tests/RedBlackTree.test.c"



int main(void) {
    int fail_count = 0;

    // Framework Test Suite ----------------------------------------------
    BUILD_SUITE("SystemCheck");
    ADD_TEST(unit_test_system_check);
    RUN_SUITE(fail_count);

    // Memory Test Suite -------------------------------------------------
    BUILD_SUITE("Memory");
    ADD_TEST(struct_has_correct_members_RedBlackTree_t);
    ADD_TEST(struct_is_of_correct_size_RedBlackTree_t);
    RUN_SUITE(fail_count);

    
    return fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}





#endif //UNIT_TEST_RUNNER_H
