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



/** ------------------------------------------------------------------------------------------------
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
 *--------------------------------------------------------------------------------------------------
 */

// Test Includes: Manually include your code here.  (keep it in alphabetical order)

// Algo
#include "../test/algo_tests/search.test.c"

// Bit
#include "../test/bit/bitfield.test.c"

// Core
#include "../test/core_tests/error.test.c"

// DateTime
#include "../test/datetime_test/datetime.test.c"

// Engine
#include "../test/engine_tests/PFEngineState.test.c"

// Log
#include "../test/log_tests/log.test.c"

// Memory
#include "../test/memory_tests/PFCompactHashTable.test.c"
#include "../test/memory_tests/HashTable.test.c"
#include "../test/memory_tests/LinkedList.test.c"
#include "../test/memory_tests/PFMemoryArena.test.c"
#include "../test/memory_tests/PFAllocator.test.c"
#include "../test/memory_tests/RedBlackTree.test.c"

// os
#include "../test/os_tests/os_utility.test.c"

// resource
#include "../test/resource_tests/PFManagedResource.test.c"
#include "../test/resource_tests/PFResourceManager.test.c"

// string
#include "../test/string_tests/PString.test.c"
#include "../test/string_tests/PFStringInternmentSingleton.test.c"



// Game Tests

// LittlestNecromancer
#include "../test/_game/slain_tests/SlainGameMode.test.c"







int main(void) {
    /*  Setting NO_FORK here allows us to ensure that check does not spin up child processes to handle
     *  test execution.  One the one hand, this means we're now running the tests serially (I guess),
     *  and it's slower.  But on the other hand, we can set break points in the tests, and debug them, and
     *  that's easily worth the slowdown.  If we need to, we can leave this commented out in the depot.
     *  This can also be set in the IDE's env vars, but I wanted someplace that I knew would make it
     *  into the depot.  EBarrett
     */
    //enum fork_status const fs = CK_NOFORK;
    //srunner_set_fork_status(global_runner, fs)

#ifdef __GNUC__
    printf("Using GNUC compiler\n\n");
#endif

    printf("\n\n---------------------\n| Engine Code Tests |\n---------------------\n\n");
    
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
    // Core Test Suite                                                     //
    // -------------------------------------------------------------------//
    BUILD_SUITE("Core");

    //--------------//
    // Core         //
    //--------------//

    // build_and_set_error_message
    //ADD_TEST(macro_build_and_set_error_message__is_defined);
    
    // get_error_buffer_size
    ADD_TEST(fn_get_error_buffer_size__is_defined);
    ADD_TEST(fn_get_error_buffer_size__has_expected_value);
    // get_error
    ADD_TEST(fn_get_error__is_defined);
    ADD_TEST(fn_get_error__returns_ptr_to_error_buffer);
    ADD_TEST(fn_get_error__returns_nullptr__if_errors_are_suppressed);
    
    // set_error
    ADD_TEST(fn_set_error__is_defined);
    ADD_TEST(fn_set_error__copies_at_most__error_buffer_size_characters);
    ADD_TEST(fn_set_error__does_not_automatically_unspresses_errors);
    // clear_error
    ADD_TEST(fn_clear_error__is_defined);
    ADD_TEST(fn_clear_error__fills_buffer_with_zeroes); 
    // set_error_suppressed
    ADD_TEST(fn_pf_set_error_suppressed__is_defined);
    ADD_TEST(fn_pf_set_error_suppressed__turns_suppression_on); 
    // set_error_not_suppressed
    ADD_TEST(fn_pf_set_error_not_suppressed__is_defined);
    ADD_TEST(fn_pf_set_error_not_suppressed__turns_suppression_off); 
    // get_is_error_suppressed
    ADD_TEST(fn_pf_get_is_error_suppressed__is_defined);
    ADD_TEST(fn_pg_get_is_error_suppressed__returns_current_state_of_suppression);
    
    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End Core Test Suite                                                //
    // -------------------------------------------------------------------//


    // -------------------------------------------------------------------//
    // DateTime Test Suite                                                //
    // -------------------------------------------------------------------//
    BUILD_SUITE("DateTime");

    // fn get_datetime_string
    ADD_TEST(fn_get_datetime_string__is_defined);
    ADD_TEST(fn_get_datetime_string__requires_buffer_len_greater_or_equal_to_22);
    ADD_TEST(fn_get_datetime_string__returns_correct_error_code__for_null_out_buffer);
    ADD_TEST(fn_get_date_time_string__sets_correct_error_message__for_null_out_buffer);
    ADD_TEST(fn_get_date_time_string__returns_correct_error_code__for_invalid_out_buffer_length);
    ADD_TEST(fn_get_date_time_string__sets_correct_error_message__for_invalid_out_buffer_length);
    ADD_TEST(fn_get_datetime_string__can_return_valid_string);
    ADD_TEST(fn_get_datetime_string__returns_correct_error_code__for_successful_use);
    
    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End DateTime Test Suite                                            //
    // -------------------------------------------------------------------//


    // -------------------------------------------------------------------//
    // Engine Test Suite                                            //
    // -------------------------------------------------------------------//
    BUILD_SUITE("Engine");


    // struct PFEngineState_t
    ADD_TEST(fn_struct_PFEngineState_t__is_defined);
    ADD_TEST(fn_struct_PFEngineState_t__has_expected_size);
    ADD_TEST(fn_struct_PFEngineState_t__has_expected_members);

    // fn pf_engine_state_initialize
    ADD_TEST(fn_pf_engine_state_initialize__is_defined);
    ADD_TEST(fn_pf_engine_state_initialize__correctly_initializes_engine_state);
    
    // fn pf_engine_state_destroy
    ADD_TEST(fn_pf_engine_state_destroy__is_defined);
    
    // fn pf_engine_state_save
    ADD_TEST(fn_pf_engine_state_save__is_defined);

    // fn pf_engine_state_load
    ADD_TEST(fn_pf_engine_state_load__is_defined);

    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End Engine Test Suite                                            //
    // -------------------------------------------------------------------//



    
    // -------------------------------------------------------------------//
    // Log Test Suite                                                     //
    // -------------------------------------------------------------------//
    BUILD_SUITE("Log");


    
    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End Log Test Suite                                                 //
    // -------------------------------------------------------------------//
    

    
    
    // -------------------------------------------------------------------//
    // Memory Test Suite                                                  //
    // -------------------------------------------------------------------//
    BUILD_SUITE("Memory");


    // -----------------//
    // CompactHashTable //
    // -----------------//

    // defines
    ADD_TEST(define_DELETED_ENTRY__is_defined);
    ADD_TEST(typedef_StringCopyFunction__is_defined);
    ADD_TEST(typedef_StringCopyFunction__can_be_set_to_strncpy);

    // helper fns
    ADD_TEST(fn_is_deleted_entry_key__is_defined);
    ADD_TEST(fn_is_deleted_entry_key__returns_false__for_null_key);
    ADD_TEST(fn_is_deleted_entry_key__returns_false__for_no_match);
    ADD_TEST(fn_is_deleted_entry_key__returns_true__for_match);
    
    
    // hash fns
    ADD_TEST(fn_interface_HashFunction__is_defined);
    ADD_TEST(fn_hash_polynomial_64__is_defined);
    ADD_TEST(fn_hash_polynomial_64__returns_uint64_max__for_zero_table_length);

    // struct CompactHashTableEntry_t
    ADD_TEST(struct_CompactHashTableEntry_t__is_defined);
    ADD_TEST(struct_CompactHashTableEntry_t__is_expected_size);
    
    // struct CompactHashTable_t
    ADD_TEST(struct_CompactHashTable_t__is_defined);
    ADD_TEST(struct_CompactHashTable_t__is_expected_size); 
    

    // compact_hash_table_create
    ADD_TEST(fn_compact_hash_table_create__is_defined);
    ADD_TEST(fn_compact_hash_table_create__allocates_memory_for_hash_table);
    ADD_TEST(fn_compact_hash_table_create__correctly_sets_starting_params);
    ADD_TEST(fn_compact_hash_table_create__allocates_a_32byte_multiple_size_for_hash_table);

    // compact_hash_table_destroy
    ADD_TEST(fn_compact_hash_table_destroy__is_defined);
    ADD_TEST(fn_compact_hash_table_destroy__will_destroy_the_table_correctly);

    // compact_hash_table_print
    ADD_TEST(fn_compact_hash_table_print_is_defined);
    //ADD_TEST(fn_compact_hash_table_print__test_usage);
    
    // compact_hash_table_insert
    ADD_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for__null_key); 
    ADD_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for_key_len_zero);
    ADD_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for_null_value);
    ADD_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for_data_type_undefined);
    ADD_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for_reserved_deleted_entry_as_key);
    ADD_TEST(fn_compact_hash_table_insert__returns_key_of_existing_value__if_value_is_already_in_table); 
    ADD_TEST(fn_compact_hash_table_insert__returns_null__if_called_when_table_is_100_percent_full);
    ADD_TEST(fn_compact_hash_table_insert__returns_null__if_key_string_could_not_be_duplicated);
    ADD_TEST(fn_compact_hash_table_insert__modifies_existing_value__if_value_is_already_in_table);

    
    // fn compact_hash_table_lookup
    ADD_TEST(fn_compact_hash_table_lookup__is_defined);
    ADD_TEST(fn_compact_hash_table_lookup__returns_null__for_null_ptr_to_table);
    ADD_TEST(fn_compact_hash_table_lookup__returns_null__for_null_ptr_to_key);
    ADD_TEST(fn_compact_hash_table_lookup__returns_null__for_key_length_zero);
    ADD_TEST(fn_compact_hash_table_lookup__returns_null__for_key_not_found_in_table); 
    ADD_TEST(fn_compact_hash_table_lookup__returns_stored_value__for_correctly_looked_up_value);
    
    
    // fn compact_hash_table_resize
    ADD_TEST(fn_compact_hash_table_resize__is_defined);
    ADD_TEST(fn_compact_hash_table_resize__returns_null__for_null_hash_table_arg);
    ADD_TEST(fn_compact_hash_table_resize__returns_null__for_negative_increase_factor);
    ADD_TEST(fn_compact_hash_table_resize__returns_null_for_increase_factor_below_one);
    ADD_TEST(fn_compact_hash_table_resize__returns_null__for_null_hash_table_entries_ptr);
    ADD_TEST(fn_compact_hash_table_resize__returns_ptr_to_newly_allocated_hash_table);
    ADD_TEST(fn_compact_hash_table_reszie__copies_existing_entries_into_new_table);
    ADD_TEST(fn_compact_hsah_table_resize__copies_state_values_during_resize);
    
    
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
    //ADD_TEST(fn_hash_table_create_iterator__is_defined);

    // fn hash_table_next
    ADD_TEST(fn_hash_table_next__is_defined);


    //------------//
    // LinkedList //
    //------------//
    
    ADD_TEST(struct_has_correct_members__LinkedListNode_t);
    ADD_TEST(struct_is_of_correct_size__LinkedListNode_t);




    //------------------------------------------------------------------------------------------------------//
    // PFAllocator_t                                                                                        //
    //------------------------------------------------------------------------------------------------------//


    //-------------//
    // MemoryArena //
    //-------------//

    // MemoryArena_t
    ADD_TEST(struct_MemoryArena_t__is_defined);
    ADD_TEST(struct_MemoryArena_t__has_correct_members);
    ADD_TEST(struct_MemoryArena_t__is_correct_size);

    // fn memory_arena_set_bytes_to_zero
    ADD_TEST(fn_pf_memory_arena_set_bytes_to_zero__is_defined);
    ADD_TEST(fn_pf_memory_arena_set_bytes_to_zero__does_not_overflow_requested_size);

    // fn memory_arena_create_with_memory
    ADD_TEST(fn_pf_memory_arena_create_with_memory__is_defined);
    ADD_TEST(fn_pf_memory_arena_create_with_memory__uses_memory_to_create_new_arena_correctly);

    // fn pf_memory_arena_push_size
    ADD_TEST(fn_pf_memory_arena_push_size__is_defined);
    ADD_TEST(fn_pf_memory_arena_push_size__returns_null__for_null_ptr_to_arena);
    ADD_TEST(fn_pf_memory_arena_push_size__returns_null__for_not_enough_memory);
    ADD_TEST(fn_pf_memory_arena_push_size__returns_null__for_zero_request);
    ADD_TEST(fn_pf_memory_arena_push_size__writes_correct_error__for_null_ptr_to_arena);
    ADD_TEST(fn_pf_memory_arena_push_size__writes_correct_error__for_not_enough_memory);
    ADD_TEST(fn_pf_memory_arena_push_size__writes_correct_error__for_zero_request);

    // macro PF_PUSH_STRUCT 
    ADD_TEST(fn_macro_PF_PUSH_STRUCT__works);

    // macro PF_PUSH_STRUCT
    ADD_TEST(fn_macro_PF_PUSH_ARRAY__works);


    //----------------------//
    // PFAllocator FreeList //
    //----------------------//

    // EAllocationPolicy_FreeList
    ADD_TEST(enum_EAllocationPolicy_FreeList__is_defined);
    ADD_TEST(enum_EAllocationPolicy_FreeList__has_expected_values);

    // PFAllocator_FreeListAllocationHeader_t
    ADD_TEST(struct_PFAllocator_FreeListAllocationHeader_t__is_defined);
    ADD_TEST(struct_PFAllocator_FreeListAllocationHeader_t__has_expected_size);
    ADD_TEST(struct_PFAllocator_FreeListAllocationHeader_t__has_expected_members);

    // PFAllocator_FreeListNode_t
    ADD_TEST(struct_PFAllocator_FreeListNode_t__is_defined);
    ADD_TEST(struct_PFAllocator_FreeListNode_t__has_expected_size);
    ADD_TEST(struct_PFAllocator_FreeListNode_t__has_expected_members);

    // fn pf_allocator_free_list_node_is_allocated
    ADD_TEST(fn_pf_allocator_free_list_node_is_allocated__is_defined);
    ADD_TEST(fn_pf_allocator_free_list_node_is_allocated__returns_correct_value_for_allocated_node);
    ADD_TEST(fn_pf_allocator_free_list_node_is_allocated__returns_correct_value_for_unallocated_node);
    ADD_TEST(fn_pf_allocator_free_list_node_is_allocated__returns_correct_error_code__for_null_ptr_to_node_param);
    ADD_TEST(fn_pf_allocator_free_list_node_is_allocated__sets_correct_error_message__for_null_ptr_to_node_param);

    // fn pf_allocator_free_list_node_set_is_allocated
    ADD_TEST(fn_pf_allocator_free_list_node_set_is_allocated__is_defined);
    ADD_TEST(fn_pf_allocator_free_list_node_set_is_allocated__sets_correct_error_message__for_null_ptr_to_node_param);
    ADD_TEST(fn_pf_allocator_free_list_node_set_is_allocated__sets_value_correctly__for_successful_use);
    ADD_TEST(fn_pf_allocator_free_list_node_set_is_not_allocated__works_correctly_without_distrubing_other_bits__when_called);

    // fn pf_allocator_free_list_node_set_is_not_allocated
    ADD_TEST(fn_pf_allocator_free_list_node_set_is_not_allocated__is_defined);
    ADD_TEST(fn_pf_allocator_free_list_node_set_is_not_allocated__sets_correct_error_message__for_null_ptr_to_node);
    ADD_TEST(fn_pf_allocator_free_list_node_set_is_not_allocated__sets_value_correctly__for_successful_use);

    // fn pf_allocator_free_list_node_get_block_size
    ADD_TEST(fn_pf_allocator_free_list_node_get_block_size__is_defined);
    ADD_TEST(fn_pf_allocator_free_list_node_get_block_size__returns_correct_block_size__without_disturbing_other_metadata);
    ADD_TEST(fn_pf_allocator_free_list_node_get_block_size__returns_correct_error_code__for_null_ptr_to_node);
    ADD_TEST(fn_pf_allocator_free_list_node_get_block_size__sets_correct_error_message__for_null_ptr_to_node);

    // fn pf_allocator_free_list_node_set_block_size
    ADD_TEST(fn_pf_allocator_free_list_node_set_block_size__is_defined);
    ADD_TEST(fn_pf_allocator_free_list_node_set_block_size__returns_correct_error_code__for_null_ptr_to_node);
    ADD_TEST(fn_pf_allocator_free_list_node_set_block_size__sets_correct_error_message__for_null_ptr_to_node);
    ADD_TEST(fn_pf_allocator_free_list_node_set_block_size__returns_correct_error_code__for_too_large_block_size);
    ADD_TEST(fn_pf_allocator_free_list_node_set_block_size__sets_correct_error_message__for_too_large_block_size);
    ADD_TEST(fn_pf_allocator_free_list_node_set_block_size__sets_block_size_correctly_and_does_not_disturb_other_bits__when_called);

    //// fn pf_allocator_free_list_node_get_padding
    ADD_TEST(fn_pf_allocator_free_list_node_get_padding_is_defined);
    ADD_TEST(fn_pf_allocator_free_list_node_get_padding__returns_correct_error_code__for_null_ptr_to_node);
    ADD_TEST(fn_pf_allocator_free_list_node_get_padding__sets_correct_error_message__for_null_ptr_to_node);
    ADD_TEST(fn_pf_allocator_free_list_node_get_padding__returns_correct_padding_value__during_correct_use);
    
    // PFAllocator_FreeList_t
    ADD_TEST(struct_PFAllocator_FreeList_t__is_defined);
    ADD_TEST(struct_PFAllocator_FreeList_t__has_expected_size);
    ADD_TEST(struct_PFAllocator_FreeList_t__has_expected_members);

    // fn pf_allocator_free_list_initialize
    ADD_TEST(fn_pf_allocator_free_list_initialize__is_defined);
    ADD_TEST(fn_pf_allocator_free_list_initialize__returns_correct_error_code__for_null_free_list_param);
    ADD_TEST(fn_pf_allocator_free_list_initialize__sets_correct_error_message__for_null_free_list_param);
    ADD_TEST(fn_pf_allocator_free_list_initialize__returns_correct_error_code__for_null_base_memory_param);
    ADD_TEST(fn_pf_allocator_free_list_initialize__sets_correct_error_message__for_null_base_memory_param);
    ADD_TEST(fn_pf_allocator_free_list_initialize__returns_correct_error_code__for_zero_size_param);
    ADD_TEST(fn_pf_allocator_free_list_initialize__sets_correct_error_message__for_zero_size_param);
    ADD_TEST(fn_pf_allocator_free_list_initialize__sets_pf_memory_functions__when_used);
    ADD_TEST(fn_pf_allocator_free_list_initialize__sets_memory_values_correctly);
    ADD_TEST(fn_pf_allocator_free_list_initialize__after_initialization_allocator_memory_fns_are_usable);
    
    // fn pf_allocator_free_list_free_all
    ADD_TEST(fn_pf_allocator_free_list_free_all__is_defined);

    ADD_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_param);
    ADD_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_param);

    ADD_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_base_memory_ptr_in_free_list);
    ADD_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_base_memory_ptr_in_free_list);

    ADD_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_zero_owned_memory_in_free_list);
    ADD_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_zero_owned_memory_in_free_list);

    ADD_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_ptr_to_pf_malloc);
    ADD_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_ptr_to_pf_malloc);

    ADD_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_ptr_to_pf_realloc);
    ADD_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_ptr_to_pf_realloc)

    ADD_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_ptr_to_pf_free);
    ADD_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_ptr_to_pf_free);

    ADD_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__when_free_list_allocator_lives_inside_its_own_base_memory);
    ADD_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__when_free_list_allocator_lives_outside_its_own_base_memory);
    
    // fn pf_allocator_is_power_of_two
    ADD_TEST(fn_pf_allocator_is_power_of_two__is_defined);

    // fn pf_allocator_free_list_calculate_padding_and_header
    ADD_TEST(fn_pf_allocator_free_list_calculate_padding_and_header__is_defined);

    // fn pf_allocator_free_list_find_first
    ADD_TEST(fn_pf_allocator_free_list_find_first__is_defined);

    // fn pf_allocator_free_list_find_best
    ADD_TEST(fn_pf_allocator_free_list_find_best__is_defined);




    
    //--------------------------//
    // PFAllocator RedBlackTree //
    //--------------------------//



    //-----------------------//
    // PFAllocator Interface //
    //-----------------------//


    // static fn ptrs
    //ADD_TEST(static_fn_ptr_pf_malloc__is_defined);
    //ADD_TEST(static_fn_ptr_pf_realloc__is_defined);
    //ADD_TEST(static_fn_ptr_pf_free_is_defined);
    //ADD_TEST(static_fn_ptrs__can_be_used__if_they_are_set_with_stdlib_defaults);


    // pf_allocator_initialize
    //ADD_TEST(fn_pf_allocator_initialize__is_defined);
    //ADD_TEST(fn_pf_allocator_initialize__returns_correct_error_code__for_null_ptr_to_base_memory);
    //ADD_TEST(fn_pf_allocator_initialize__sets_correct_error_message__for_null_ptr_to_base_memory);
    //ADD_TEST(fn_pf_allocator_initialize__returns_correct_error_code__for_invalid_memory_size);
    //ADD_TEST(fn_pf_allocator_initialize__sets_correct_error_message__for_invalid_memory_size);



    //------------------------------------------------------------------------------------------------------//
    // End PFAllocator_t                                                                                    //
    //------------------------------------------------------------------------------------------------------//
    

    //--------------//
    // RedBlackTree //
    //--------------//

    ADD_TEST(enum_has_correct_members__ERedBlackTreeNodeColor_t);
    ADD_TEST(enum_has_correct_members__ERedBlackTreeNodeDirection_t);
    ADD_TEST(struct_has_correct_members__RedBlackTree_t);
    ADD_TEST(struct_is_of_correct_size__RedBlackTree_t);
    ADD_TEST(struct_has_correct_members__RedBlackTreeNode_t);
    

    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End Memory Test Suite                                              //
    // -------------------------------------------------------------------//



    // -------------------------------------------------------------------//
    // OS Test Suite                                                      //
    // -------------------------------------------------------------------//
    BUILD_SUITE("OS");

    ADD_TEST(fn_file_size__is_defined);
    ADD_TEST(fn_file_size__returns_correct_error_code__for_null_path);
    ADD_TEST(fn_file_size__returns_correct_error_code__for_zero_path_length);
    ADD_TEST(fn_file_size__returns_correct_error_code__if_file_does_not_exist);
    ADD_TEST(fn_file_size__writes_correct_error__for_null_path);
    ADD_TEST(fn_file_size__writes_correct_error__for_zero_path_length);
    ADD_TEST(fn_file_size__writes_correct_error__if_file_does_not_exist);
    
    ADD_TEST(fn_is_file__is_defined);
    ADD_TEST(fn_is_file__returns_correct_error_code__for_null_path);
    ADD_TEST(fn_is_file__returns_correct_error_code__for_zero_path_length);
    ADD_TEST(fn_is_file__returns_zero__if_file_does_not_exist);
    ADD_TEST(fn_is_file__returns_one__if_file_does_exist);
    ADD_TEST(fn_is_file__writes_correct_error__for_null_path);
    ADD_TEST(fn_is_file__writes_correct_error__for_zero_path_length);
    
    ADD_TEST(fn_is_directory__is_defined);
    ADD_TEST(fn_is_directory__returns_correct_error_code__for_null_path);
    ADD_TEST(fn_if_directory__returns_correct_error_code__for_zero_path_length);
    ADD_TEST(fn_is_directory__returns_zero__if_path_is_not_a_directory);
    ADD_TEST(fn_is_directory__returns_one__if_path_is_a_directory);
    ADD_TEST(fn_is_directory__writes_correct_error__for_null_path);
    ADD_TEST(fn_is_directory__writes_correct_error__for_zero_path_length);
    
    RUN_SUITE(fail_count); 
    // -------------------------------------------------------------------//
    // End OS Test Suite                                                  //
    // -------------------------------------------------------------------//

    // -------------------------------------------------------------------//
    // Start Resource Test Suite                                          //
    // -------------------------------------------------------------------//
    BUILD_SUITE("Resource");

    //---------------------//
    // PFManagedResource_t //
    //---------------------//
    ADD_TEST(fn_struct_PFManagedResource_t__is_defined);
    ADD_TEST(fn_struct_PFManagedResource_t__has_expected_size);
    ADD_TEST(fn_struct_PFManagedResource_t__has_expected_members);


    //---------------------//
    // PFResourceManager_t //
    //---------------------//
    ADD_TEST(fn_struct_PFResourceManager_t__is_defined);
    ADD_TEST(fn_struct_PFResourceManager_t__has_expected_size);

    // pf_resource_manager_initialize
    ADD_TEST(fn_pf_resource_manager_initialize__is_defined);
    //ADD_TEST(fn_pf_resource_manager_initialize__initializes_resource_manager_allocator_when_called);

    // pf_resource_manager_register_resource_with_path
    ADD_TEST(fn_pf_resource_manager_register_resource_with_path__is_defined);

    // pf_resource_manager_load_resource_with_name
    ADD_TEST(fn_pf_resource_manager_load_resource_with_name__is_defined);

    // pf_resource_manager_get_resource_with_name
    ADD_TEST(fn_pf_resource_manager_get_resource_with_name__is_defined);

    // pf_resource_manager_unload_resource_with_name
    ADD_TEST(fp_pf_resource_manager_unload_resource_with_name__is_defined);

    // pf_resource_manager_unregister_resource_with_name
    ADD_TEST(fn_pf_resource_manager_unregister_resource_with_name__is_defined);




    
    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End Resource Test Suite                                            //
    // -------------------------------------------------------------------//
    

    // -------------------------------------------------------------------//
    // String Test Suite                                                  //
    // -------------------------------------------------------------------//
    BUILD_SUITE("String");

    //-----------//
    // PString_t //
    //-----------//
    
    ADD_TEST(struct_PString_t__is_defined);
    ADD_TEST(struct_PString_t__has_expected_size);

    // fn pstring_contains_char_sub    
    ADD_TEST(fn_pstring_constains_char_sub__is_defined);

    // fn pstring_contains_pstr_sub
    // this fn actually calls into the char_sub version, so we're saying it's "testing both"
    ADD_TEST(fn_pstring_contains_pstr_sub__is_defined);
    ADD_TEST(fn_pstring_contains_pstr_sub__returns_false__if_substring_is_longer_that_first_string);
    ADD_TEST(fn_pstring_contains_pstr_sub__logs_correct_error__if_substring_is_longer_than_first_string);
    ADD_TEST(fn_pstring_contains_pstr_sub__can_find_a_substring); 
    ADD_TEST(fn_pstring_contains_pstr_sub__stress_testing);

    // fn pstring_slice
    ADD_TEST(fn_pstring_slice__is_defined);
    ADD_TEST(fn_pstirng_slice__returns_null_result__of_correct_form);
    ADD_TEST(fn_pstring_slice__returns_null_result__for_pstr_arg_with_null_string);
    ADD_TEST(fn_pstring_slice__sets_correct_error_message__for_pstring_arg_with_null_string);
    ADD_TEST(fn_pstring_slice__returns_null_result__for_pstr_arg_with_zero_length);
    ADD_TEST(fn_pstring_slive__sets_correct_error_message__for_pstr_arg_with_zero_length);
    ADD_TEST(fn_pstring_slice__returns_null_result__for_request_of_zero_length_slice);
    ADD_TEST(fn_pstring_slice__sets_correct_error_message__for_request_of_zero_length_slice);
    ADD_TEST(fn_pstring_slice__returns_null_result__for_out_of_bounds_begin_idx);
    ADD_TEST(fn_pstring_slice__sets_correct_error_message__for_out_of_bounds_begin_idx);
    ADD_TEST(fn_pstring_slice__returns_null_result__for_out_of_bounds_end_idx);
    ADD_TEST(fn_pstring_slice__sets_correct_error_message__for_out_of_bounds_end_idx);
    //ADD_TEST(fn_pstring_slice__sets_correct_error_message__for_hack_slice_which_creates_null_begin_ptr);
    ADD_TEST(fn_pstring_slice__returns_non_owning_pstring__for_valid_slice);
    ADD_TEST(fn_pstring_slice__returns_slice__when_using_negative_indicies);
    ADD_TEST(fn_pstring_slice__returns_slice__when_using_positive_then_negative_indicies);
    ADD_TEST(fn_pstring_slice__feels_okay_in_casual_usage);


    //-------------------------------//
    // PFStringInternmentSingleton_t //
    //-------------------------------//

    // constants
    ADD_TEST(constant_PFSI_MAX_STRINGS__is_expected_value);

    // struct PFStringInternmentSingleton_t
    ADD_TEST(struct_PFStringInternmentSingleton_t__is_defined);
    ADD_TEST(struct_PFStringInternmentSingleton_t__has_correct_size);
    ADD_TEST(struct_PFStringInternmentSingleton_t__has_expected_members);

    // fn pf_string_internment_initialize
    ADD_TEST(fn_pf_string_internment_initialize__is_defined);
    ADD_TEST(fn_pf_string_internment_initialize__returns_correct_error_code__for_null_string_internment_param);
    ADD_TEST(fn_pf_string_internment_initialize__sets_correct_error_message__for_null_string_internment_param);
    ADD_TEST(pf_string_internment_initialize__returns_correct_error_code__for_null_memory_base_param);
    ADD_TEST(pf_string_internment_initialize__sets_correct_error_message__for_null_memory_base_param);
    ADD_TEST(pf_string_internment_initialize__returns_correct_error_code__for_invalid_memory_size);
    ADD_TEST(pf_string_internment_initialize__sets_correct_error_message__for_invalid_memory_size);
    ADD_TEST(pf_string_internment_initialize__correctly_initializes_string_internment__for_expected_arguments);

    // fn pf_string_internment_emplace_cstring
    ADD_TEST(fn_pf_string_internment_emplace_cstring__is_defined);

    // first param
    ADD_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_null_ptr_to_PFStringInternmentSingleton_t);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_null_ptr_to_PFStirngInternmentSingleton_t);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_null_ptr_to_memory_base_in_PFStringInternmentSingleton_t);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_null_ptr_to_memory_base_in_PFStringInternmentSingleton_t);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_invalid_owned_memory_size_in_PFStringInternmentSingleton_t);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_invalid_owned_memory_size_in_PFStringInternmentSingleton_t);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_used_memory_size_greather_than_owned_memory_size_PFStringInternmentSingleton_t);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_used_memory_sie_greater_than_owned_memory_size_in_PFStringInternmentSingleton_t);

    // second param
    ADD_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_null_ptr_to_cstring);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_null_ptr_to_cstring);

    // third param
    ADD_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_invalid_length);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_invalid_length);

    // fn internal
    ADD_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_out_of_memory_error);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_out_of_memory_error);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_out_of_tracking_indices);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_out_of_tracking_indices);

    ADD_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_pstr__for_valid_args);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__copies_string_to_memory__for_valid_args);
    ADD_TEST(fn_pf_string_internment_emplace_cstr__sets_memory_used_correctly_after_copy);


    
    // fn pf_string_internment_emplace_pstring
    //ADD_TEST(fn_pf_string_internment_emplace_pstring__is_defined);

    
    RUN_SUITE(fail_count);
    // -------------------------------------------------------------------//
    // End String Test Suite                                              //
    // -------------------------------------------------------------------//



    // -------------------------------------------------------------------//
    //                                                                    //
    // -------------------------------------------------------------------//
    //                                                                    //
    // -------------------------------------------------------------------//
    //                                                                    //
    // -------------------------------------------------------------------//
    //                                                                    //
    // -------------------------------------------------------------------//

    printf("\n\n-----------------------\n| Gameplay Code Tests |\n-----------------------\n\n");


    
    // -------------------------------------------------------------------//
    // Start Slain Test Suite                                             //
    // -------------------------------------------------------------------//

    printf("Slain\n\n");
    
    BUILD_SUITE("SlainGameMode_t");

    //---------------//
    // ELNGameMode_t //
    //---------------//
    // ELLIETODO: Rename
    
    ADD_TEST(fn_enum_ESlainGameMode_t__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__has_expected_number_of_states);
    ADD_TEST(fn_enum_ELNGameMode_t__application_startup_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__application_main_menu_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__application_demo_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__application_loading_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__application_settings_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__application_about_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_pause_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_character_creator_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_decorate_area_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_dialogue_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_inventory_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_npc_friends_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_quest_tracker_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_trade_value__is_defined);
    ADD_TEST(fn_enum_ELNGameMode_t__gameplay_world_map_navigation_value__is_defined);




    
    RUN_SUITE(fail_count);
    
    // -------------------------------------------------------------------//
    // End Slain Test Suite                                               //
    // -------------------------------------------------------------------//




    

    

    
    
    return fail_count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}





#endif //UNIT_TEST_RUNNER_H
