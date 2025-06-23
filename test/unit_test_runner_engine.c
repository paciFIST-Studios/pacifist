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

// Algo
#include "../test/algo_tests/search.test.c"

// Bit
#include "../test/bit/bitfield.test.c"

// Memory
#include "../test/memory_tests/RedBlackTree.test.c"
#include "../test/memory_tests/CompactHashTable.test.c"
#include "../test/memory_tests/HashTable.test.c"
#include "../test/memory_tests/LinkedList.test.c"
#include "../test/memory_tests/MemoryArena.test.c"


int main(void) {
    /*  Setting NO_FORK here allows us to ensure that check does not spin up child processes to handle
     *  test execution.  One the one hand, this means we're now running the tests serially (I guess),
     *  and it's slower.  But on the other hand, we can set break points in the tests, and debug them, and
     *  that's easily worth the slowdown.  If we need to, we can leave this commented out in the depot.
     *  This can also be set in the IDE's env vars, but I wanted someplace that I knew would make it
     *  into the depot.  EBarrett
     */
    //enum fork_status const fs = CK_NOFORK;
    //srunner_set_fork_status(global_runner, fs);

    int fail_count = 0;
    
    // -------------------------------------------------------------------//
    // Framework Test Suite                                               //
    // -------------------------------------------------------------------//
    BUILD_SUITE("SystemCheck");
    ADD_TEST(unit_test_system_check);
    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End Framework Test Suite                                           //
    // -------------------------------------------------------------------//

    
    // -------------------------------------------------------------------//
    // Algorithm Test Suite                                               //
    // -------------------------------------------------------------------//
    BUILD_SUITE("Algo")
    ADD_TEST(stdlib_bsearch_example__int32);
    RUN_SUITE(fail_count); 
    // -------------------------------------------------------------------//
    // End Algorithm Test Suite                                           //
    // -------------------------------------------------------------------//

    
    // -------------------------------------------------------------------//
    // Bit Test Suite                                                     //
    // -------------------------------------------------------------------//
    BUILD_SUITE("Bit");
    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End Bit Test Suite                                                 //
    // -------------------------------------------------------------------//

    
    // -------------------------------------------------------------------//
    // Memory Test Suite                                                  //
    // -------------------------------------------------------------------//
    BUILD_SUITE("Memory");

    //--------------//
    // RedBlackTree //
    //--------------//

    ADD_TEST(enum_has_correct_members__ERedBlackTreeNodeColor_t);
    ADD_TEST(enum_has_correct_members__ERedBlackTreeNodeDirection_t);
    ADD_TEST(struct_has_correct_members__RedBlackTree_t);
    ADD_TEST(struct_is_of_correct_size__RedBlackTree_t);
    ADD_TEST(struct_has_correct_members__RedBlackTreeNode_t);

    // -----------------//
    // CompactHashTable //
    // -----------------//

    // defines
    ADD_TEST(define_DELETED_ENTRY__is_defined);
    ADD_TEST(typedef_StringCopyFunction__is_defined);
    ADD_TEST(typedef_StringCopyFunction__can_be_set_to_strncpy);

    
    // hash fns
    ADD_TEST(fn_interface_HashFunction__is_defined);
    ADD_TEST(fn_hash_polynomial_64__is_defined);
    ADD_TEST(fn_hash_polynomial_64__returns_uint64_max__for_zero_table_length);
    
    // CompactHashTable_t
    ADD_TEST(struct_CompactHashTable_t__is_defined);
    ADD_TEST(fn_compact_hash_table_create__is_defined);
    ADD_TEST(fn_compact_hash_table_create__allocates_memory_for_hash_table);

    // compact_hash_table_create
    ADD_TEST(fn_compact_hash_table_create__is_defined);
    ADD_TEST(fn_compact_hash_table_create__allocates_memory_for_hash_table);
    ADD_TEST(fn_compact_hash_table_create__correctly_sets_starting_params);

    // compact_hash_table_destroy
    ADD_TEST(fn_compact_hash_table_destroy__is_defined);
    ADD_TEST(fn_compact_hash_table_destroy__will_destroy_the_table_correctly);
    
    
    // compact_hash_table
    

    // ----------//
    // HashTable //
    // ----------//

    // struct HashTableEntry_t
    ADD_TEST(struct_HashTableEntry_t__is_defined);
    ADD_TEST(struct_HashTableEntry_t__has_correct_properties);
    ADD_TEST(struct_HashTableEntry_t__has_correct_size);

    // struct HashTable_t
    ADD_TEST(struct_HashTable_t__is_defined);
    ADD_TEST(struct_HashTable_t__has_correct_properties);

    // fn hash_table_create
    ADD_TEST(fn_hash_table_create__is_defined);
    ADD_TEST(fn_hash_table_create__returns_valid_hash_table);
    ADD_TEST(fn_hash_table_create__returns_null__for_nomem_cannot_create_table);
    ADD_TEST(fn_hash_table_create__returns_null__for_nomem_cannot_create_elements);

    // fn hash_table_destroy
    ADD_TEST(fn_hash_table_destroy__is_defined);

    // fn hash_table_expand
    ADD_TEST(fn_hash_table_expand__is_defined);
    
    // fn hash_table_lookup
    ADD_TEST(fn_hash_table_lookup__is_defined);

    // fn hash_table_insert
    ADD_TEST(fn_hash_table_insert__is_defined);

    // fn hash_table_count
    ADD_TEST(fn_hash_table_count__is_defined);

    // struct HashTableIterator_t
    ADD_TEST(struct_HashTableIterator_t__is_defined);

    // fn hash_table_create_iterator
    ADD_TEST(fn_hash_table_create_iterator__is_defined);

    // fn hash_table_next
    ADD_TEST(fn_hash_table_next__is_defined);


    //------------//
    // LinkedList //
    //------------//
    
    ADD_TEST(struct_has_correct_members__LinkedListNode_t);
    ADD_TEST(struct_is_of_correct_size__LinkedListNode_t);

    //-------------//
    // MemoryArena //
    //-------------//

    // MemoryArena_t
    ADD_TEST(struct_MemoryArena_t__is_defined);
    ADD_TEST(struct_MemoryArena_t__has_correct_members);
    ADD_TEST(struct_MemoryArena_t__is_correct_size);

    // fn memory_arena_create
    ADD_TEST(fn_memory_arena_create__is_defined);
    ADD_TEST(fn_memory_arena_create__returns_MemoryArena_t_with_initialized_settings);

    

    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End Memory Test Suite                                              //
    // -------------------------------------------------------------------//

    
    return fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}





#endif //UNIT_TEST_RUNNER_H
