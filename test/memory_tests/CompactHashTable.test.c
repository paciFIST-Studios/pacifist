// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "../../src/memory/hash_table/CompactHashTable.h"

// stdlib
#include <stdlib.h>
#include <string.h>
// framework
// project


// Defines -------------------------------------------------------------------------------------------------------------

START_TEST(define_DELETED_ENTRY__is_defined) {
    char const * deleted_entry = DELETED_ENTRY;
    ck_assert_ptr_nonnull(deleted_entry);
    ck_assert_int_eq(strcmp(deleted_entry, "DELETED_ENTRY_THIS_KEY_IS_INVALID"), 0);
}
END_TEST


START_TEST(typedef_StringCopyFunction__is_defined) {
    StringCopyFunction_t* p = NULL;
    ck_assert_ptr_null(p);
}
END_TEST


START_TEST(typedef_StringCopyFunction__can_be_set_to_strncpy) {
    StringCopyFunction_t* p = strndup;
    ck_assert_ptr_nonnull(p);
}
END_TEST

// Helper Functions ----------------------------------------------------------------------------------------------------

START_TEST(fn_is_deleted_entry_key__is_defined) {
    bool(*fptr)(char const*) = &is_deleted_entry_key;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_is_deleted_entry_key__returns_false__for_null_key) {
    ck_assert_int_eq(is_deleted_entry_key(NULL), false);    
}
END_TEST

START_TEST(fn_is_deleted_entry_key__returns_false__for_no_match) {
    ck_assert_int_eq(is_deleted_entry_key("some_other_key"), false);    
}
END_TEST

START_TEST(fn_is_deleted_entry_key__returns_true__for_match) {
    ck_assert_int_eq(is_deleted_entry_key(DELETED_ENTRY), true);    
}


// Hashing Functions ---------------------------------------------------------------------------------------------------

START_TEST(fn_interface_HashFunction__is_defined) {
    HashFunction_t* p = NULL;
    ck_assert_ptr_null(p);
}
END_TEST


START_TEST(fn_hash_polynomial_64__is_defined) {
    HashFunction_t* ptr = hash_polynomial_64;
    ck_assert_ptr_nonnull(ptr);
}
END_TEST


START_TEST(fn_hash_polynomial_64__returns_uint64_max__for_null_key) {
    ck_assert_int_eq(hash_polynomial_64(NULL, 5), UINT64_MAX);
}
END_TEST


START_TEST(fn_hash_polynomial_64__returns_uint64_max__for_zero_table_length) {
    ck_assert_int_eq(hash_polynomial_64("key", 0), UINT64_MAX);
}
END_TEST

// struct CompactHashTableEntry_t --------------------------------------------------------------------------------------

START_TEST(struct_CompactHashTableEntry_t__is_defined) {
    CompactHashTableEntry_t const entry = {0};
    ck_assert_ptr_nonnull(&entry);
}
END_TEST

START_TEST(struct_CompactHashTableEntry_t__is_expected_size) {
    CompactHashTableEntry_t const entry = {0};
    ck_assert_int_eq(sizeof(entry), 32);
}
END_TEST



// struct CompactHashTable_t -------------------------------------------------------------------------------------------

START_TEST(struct_CompactHashTable_t__is_defined) {
    CompactHashTable_t * table = compact_hash_table_create(0, NULL);
    ck_assert_ptr_nonnull(table);
    free(table);
}
END_TEST

START_TEST(struct_CompactHashTable_t__is_expected_size) {
    CompactHashTable_t const table = {0};
    ck_assert_int_eq(sizeof(table), 32);
}
END_TEST


// fn compact_hash_table_create ----------------------------------------------------------------------------------------

START_TEST(fn_compact_hash_table_create__is_defined) {
    CompactHashTable_t* (*fptr)(uint32_t, HashFunction_t) = &compact_hash_table_create;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_compact_hash_table_create__allocates_memory_for_hash_table) {
    CompactHashTable_t * table = NULL;
    ck_assert_ptr_null(table);
    table = compact_hash_table_create(0, NULL);
    ck_assert_ptr_nonnull(table);
    free(table);
}
END_TEST


START_TEST(fn_compact_hash_table_create__correctly_sets_starting_params) {
    CompactHashTable_t * table = compact_hash_table_create(2, hash_polynomial_64);
    ck_assert_ptr_nonnull(table);
    ck_assert_int_eq(table->size, 2);
    ck_assert_ptr_nonnull(table->hash_fn);
    ck_assert_ptr_eq(table->hash_fn, hash_polynomial_64); /* sets the supplied fn */
    ck_assert_ptr_nonnull(table->string_copy_fn);
    ck_assert_ptr_eq(table->string_copy_fn, strndup); /* sets correct default */
    ck_assert_ptr_nonnull(table->entries);

    // make sure all of the entries are correctly zeroed out before start
    size_t const table_size = table->size;
    size_t const entry_size = sizeof(CompactHashTableEntry_t);

    
    for (int32_t i = 0; i < table_size; i++) {
        /* Okay so here's the deal.  All of the memory for the hash table, is given
         * in one allocation.  The first like, 56 bytes are for the table, and the
         * rest of the memory---increments of 24 bytes---are given over to the array
         * of table entries.
         *
         * In order to iterate those entries, I think we need to manually re-cast the
         * memory, which is done here.
         *
         * TODO: add an iterator for the entries
         */
        CompactHashTableEntry_t * entry = (table->entries + i * entry_size);
        ck_assert_ptr_null(entry->key);
        ck_assert_int_eq(entry->key_len, 0);
        ck_assert_ptr_null(entry->value);
    }

    // teardown

    for (int32_t i = 0; i < table_size; i++) {
        CompactHashTableEntry_t * entry = (table->entries + i * entry_size);
        if (entry->key != NULL) {
            free(entry->key);
            entry->key = NULL;
        }
    }

    // we can't free the entries, b/c it's the same allocation as the table
    table->entries = NULL;
    free(table);
    table = NULL;
    ck_assert_ptr_null(table);
}
END_TEST


START_TEST(fn_compact_hash_table_create__allocates_a_32byte_multiple_size_for_hash_table) {
    int32_t const test_element_count = 11;
    size_t const table_size = sizeof(CompactHashTable_t);
    size_t const element_size = sizeof(CompactHashTableEntry_t);

    // this test repeats with all of these sizes. There shouldn't(TM) be a difference
    // between a table of 7 entries, and one of 11, but you never know
    for (int32_t i = 0; i < test_element_count; ++i) {
        CompactHashTable_t * ht = compact_hash_table_create(i, hash_fnv1a_64);

        uint64_t const table_addr = (uint64_t)ht;
        
        for (int32_t j = 0; j < test_element_count; ++j) {

            // checks to make sure this element is in the place we think it is
            uint64_t const element_addr_predicted = (uint64_t)(table_addr + table_size + (j * element_size)); 
            uint64_t const element_addr = (uint64_t)&(ht->entries[j]);
            ck_assert_int_eq(element_addr, element_addr_predicted);

            // checks to make sure this element is 32 byte aligned
            ck_assert((element_addr - table_addr) % 32 == 0);
        }
        
        compact_hash_table_destroy(ht);
    }
}
END_TEST


// fn compact_hash_table_destroy ---------------------------------------------------------------------------------------

START_TEST(fn_compact_hash_table_destroy__is_defined) {
    bool (*fptr)(CompactHashTable_t*) = &compact_hash_table_destroy;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_compact_hash_table_destroy__will_destroy_the_table_correctly) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_nonnull(ht);
    char* test_key = "test_key";
    char const * res = compact_hash_table_insert(ht, test_key, strlen(test_key), EPDT__int8_t,(void*) 5);
    // the key has been copied, so these pointers shouldn't be the same
    ck_assert_ptr_ne(res, test_key);
    // but they should have the same value
    ck_assert(strcmp(res, test_key) == 0);
    ck_assert_int_eq(compact_hash_table_destroy(ht), true);
}
END_TEST

// fn compact_hash_table_print -----------------------------------------------------------------------------------------

START_TEST(fn_compact_hash_table_print_is_defined) {
    void(*fptr)(CompactHashTable_t*) = &compact_hash_table_print;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_compact_hash_table_print__test_usage) {
    CompactHashTable_t* ht = compact_hash_table_create(2, hash_polynomial_64);
    compact_hash_table_insert(ht, "test", 4, EPDT__char_ptr_t, "this is just a test");
    compact_hash_table_print(ht);
    compact_hash_table_insert(ht, "test2", 5, EPDT__char_ptr_t, "this_is_also_a_test");
    compact_hash_table_print(ht);
    compact_hash_table_destroy(ht);
    compact_hash_table_print(NULL);
}
END_TEST


// fn compact_hash_table_insert ----------------------------------------------------------------------------------------

START_TEST(fn_compact_hash_table_insert__is_defined) {
    char const *(*fptr)(CompactHashTable_t*, char const *, size_t const, EProjectDataTypes_t, void*) = &compact_hash_table_insert;
    ck_assert_ptr_nonnull(fptr);    
}
END_TEST


START_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for_null_table) {

    char const * key = compact_hash_table_insert(NULL, "key", 4, 0, NULL);
    ck_assert_ptr_null(key);
    ck_assert_ptr_eq(compact_hash_table_get_error_message(), ERROR_NULL_PTR_TO_HASH_TABLE);
}
END_TEST


START_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for__null_key) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_null(compact_hash_table_insert(ht, NULL, 0, 0, NULL));
    ck_assert(strcmp(compact_hash_table_get_error_message(), ERROR_NULL_PTR_TO_KEY) == 0);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST

START_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for_key_len_zero) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    char* test_key = "test_key";
    ck_assert_ptr_null(compact_hash_table_insert(ht, test_key, 0, 0, test_key));
    ck_assert(strcmp(compact_hash_table_get_error_message(), ERROR_INVALID_KEY_LENGTH) == 0);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST

START_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for_data_type_undefined) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    char* test_key = "test_key";
    ck_assert_ptr_null(compact_hash_table_insert(ht, test_key, strlen(test_key), 0, test_key));
    ck_assert(strcmp(compact_hash_table_get_error_message(), ERROR_UNDEFINED_DATA_TYPE) == 0);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST

START_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for_null_value) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    char const * test_key = "test_key";
    size_t const key_len = strlen(test_key);
    ck_assert_ptr_null(compact_hash_table_insert(ht, test_key, key_len, EPDT__char_ptr_t, NULL));
    ck_assert(strcmp(compact_hash_table_get_error_message(), ERROR_CANNOT_STORE_NULL) == 0);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST

START_TEST(fn_compact_hash_table_insert__returns_null_and_sets_error_message_for_reserved_deleted_entry_as_key) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    char const * test_key = DELETED_ENTRY;
    size_t const key_len = strlen(test_key);
    ck_assert_ptr_null(compact_hash_table_insert(ht, test_key, key_len, EPDT__char_ptr_t, (void*)test_key));
    ck_assert(strcmp(compact_hash_table_get_error_message(), ERROR_CANT_USE_DELETED_ENTRY_AS_KEY) == 0);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST


START_TEST(fn_compact_hash_table_insert__returns_key_of_existing_value__if_value_is_already_in_table) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    char const * test_key = "test_key";
    size_t const key_len = strlen(test_key);
    char const * res1 = compact_hash_table_insert(ht, test_key, key_len, EPDT__char_ptr_t, (void*)test_key);
    char const * res2 = compact_hash_table_insert(ht, test_key, key_len, EPDT__char_ptr_t, (void*)test_key);
    // this value was already in the table, so the second insert should return the same, copied key
    ck_assert_ptr_eq(res1, res2);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST


START_TEST(fn_compact_hash_table_insert__modifies_existing_value__if_value_is_already_in_table) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    char const * test_key = "test_key";
    void * test_value = (void*)test_key;
    size_t const key_len = strlen(test_key);
    char const * res1 = compact_hash_table_insert(ht, test_key, key_len, EPDT__char_const_ptr_t, test_value);
    char const * res2 = compact_hash_table_insert(ht, test_key, key_len, EPDT__char_ptr_t, "some other value");
    // this value was already in the table, so the second insert should return the same, copied key
    ck_assert_ptr_eq(res1, res2);
    // this value should have the new value stored in it, so these should no longer match
    char const * test_value2 = (char const *)compact_hash_table_lookup(ht, test_key, key_len);
    ck_assert(strncmp(test_value, test_value2, key_len) != 0);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST


START_TEST(fn_compact_hash_table_insert__returns_null__if_called_when_table_is_100_percent_full) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    char const * test_key = "test_key";
    size_t const key_len = strlen(test_key);
    char const * res1 = compact_hash_table_insert(ht, test_key, key_len, EPDT__char_ptr_t, (void*)test_key);
    ck_assert_ptr_nonnull(res1);
    char const * another_key = "another_key";
    size_t const another_key_len = strlen(another_key);
    char const * res2 = compact_hash_table_insert(ht, another_key, another_key_len, EPDT__char_ptr_t, (void*)another_key);
    ck_assert_ptr_null(res2);
    //ck_assert_ptr_eq(get_error_message(), ERROR_TABLE_IS_ENTIRELY_FULL_BUT_HAS_NOT_BEEN_RESIZED);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST


// this mock fn is used to test a specific error condition, in the fn:
// compact_hash_table_insert.  If the key string of the newly inserted
// key-value cannot be copied, it returns NULL, and sets this error
static char * return_null(char const * src, size_t n) {
    return NULL;
}

START_TEST(fn_compact_hash_table_insert__returns_null__if_key_string_could_not_be_duplicated) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ht->string_copy_fn = &return_null; 
    
    char const * test_key = "test_key";
    size_t const key_len = strlen(test_key);
    char const * res1 = compact_hash_table_insert(ht, test_key, key_len, EPDT__char_ptr_t, (void*)test_key);
    ck_assert_ptr_null(res1);
    ck_assert(strcmp(compact_hash_table_get_error_message(), ERROR_COULD_NOT_DUPLICATE_KEY_STRING) == 0);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST


// compact_hash_table_lookup -------------------------------------------------------------------------------------------

START_TEST(fn_compact_hash_table_lookup__is_defined) {
    void*(*fptr)(CompactHashTable_t*,char const *, size_t) = &compact_hash_table_lookup;
    ck_assert_ptr_nonnull(fptr); 
}
END_TEST


START_TEST(fn_compact_hash_table_lookup__returns_null__for_null_ptr_to_table) {
    ck_assert_ptr_null(compact_hash_table_lookup(NULL, "k", 1));
}
END_TEST


START_TEST(fn_compact_hash_table_lookup__returns_null__for_null_ptr_to_key) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_nonnull(ht);
    ck_assert_ptr_null(compact_hash_table_lookup(ht, NULL, 1));
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST


START_TEST(fn_compact_hash_table_lookup__returns_null__for_key_length_zero) {
    CompactHashTable_t * ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_nonnull(ht);
    ck_assert_ptr_null(compact_hash_table_lookup(ht, "test_key", 0));
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST


START_TEST(fn_compact_hash_table_lookup__returns_null__for_key_not_found_in_table) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_nonnull(ht);
    ck_assert_ptr_nonnull(compact_hash_table_insert(ht, "k", 2, EPDT__char_ptr_t, (void*)1));
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST


START_TEST(fn_compact_hash_table_lookup__returns_stored_value__for_correctly_looked_up_value) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_nonnull(ht);
    char const * test_key = "test_key";
    size_t const test_key_len = strlen(test_key);
    char const * stored_key = compact_hash_table_insert(ht, test_key, test_key_len, EPDT__char_ptr_t, (void*)test_key);
    ck_assert_ptr_nonnull(stored_key);
    ck_assert(strncmp(test_key, stored_key, test_key_len) == 0);
    void const * value = compact_hash_table_lookup(ht, test_key, test_key_len); 
    ck_assert_ptr_nonnull(value);
    char const * stored_value = value;
    ck_assert(strncmp(test_key, stored_value, test_key_len) == 0);
    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST


// fn compact_hash_table_resize ---------------------------------------------------------------------------------------- 

START_TEST(fn_compact_hash_table_resize__is_defined) {
    CompactHashTable_t*(*fptr)(CompactHashTable_t*, float) = &compact_hash_table_resize;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_compact_hash_table_resize__returns_null__for_null_hash_table_arg) {
   ck_assert_ptr_null(compact_hash_table_resize(NULL, 2));
}
END_TEST

START_TEST(fn_compact_hash_table_resize__returns_null__for_negative_increase_factor) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_null(compact_hash_table_resize(ht, -1.0));
    compact_hash_table_destroy(ht);
}
END_TEST

START_TEST(fn_compact_hash_table_resize__returns_null_for_increase_factor_below_one) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_nonnull(ht);
    ck_assert_int_eq(ht->size, 1);
    ck_assert_int_eq(ht->used, 0);
    ck_assert_ptr_null(compact_hash_table_resize(ht, 0.f)); 
    ck_assert_int_eq(ht->size, 1);
    ck_assert_int_eq(ht->used, 0);
}
END_TEST

START_TEST(fn_compact_hash_table_resize__returns_null__for_null_hash_table_entries_ptr) {
    CompactHashTable_t ht = {0};
    ht.entries = NULL;
    ck_assert_ptr_null(compact_hash_table_resize(&ht, 2));
}
END_TEST

START_TEST(fn_compact_hash_table_resize__returns_ptr_to_newly_allocated_hash_table) {
    CompactHashTable_t* ht1 = compact_hash_table_create(1, hash_polynomial_64);
    CompactHashTable_t* ht2 = compact_hash_table_resize(ht1, 2.0f);
    ck_assert_ptr_ne(ht1, ht2);
    ck_assert(compact_hash_table_destroy(ht2));
    ht1 = NULL;
    ht2 = NULL;
}
END_TEST

START_TEST(fn_compact_hash_table_reszie__copies_existing_entries_into_new_table) {
    char const * test_key = "test_key";
    size_t const test_key_len = strlen(test_key);
    char const * test_value = "test_value";
    
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_int_eq(ht->used, 0);
    ck_assert_ptr_nonnull(compact_hash_table_insert(ht, test_key, test_key_len, EPDT__char_ptr_t, (void*)test_value));
    ck_assert_int_eq(ht->used, 1);

    ht = compact_hash_table_resize(ht, 10);
    //ck_assert_int_eq(ht->used, 1);

    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST

START_TEST(fn_compact_hsah_table_resize__copies_state_values_during_resize) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_nonnull(ht);
    ck_assert_int_eq(ht->size, 1);
    ck_assert_int_eq(ht->used, 0);
    
    //bool allow_auto_resize = true;
    //float auto_resize_percent = 0.9f;
    StringCopyFunction_t* string_copy_fn = ht->string_copy_fn;
    HashFunction_t* hash_fn = ht->hash_fn;

    ht = compact_hash_table_resize(ht, 2.0f);
    ck_assert_ptr_nonnull(ht);

    // increase %+1
    ck_assert_int_eq(ht->size, 3);
    ck_assert_int_eq(ht->used, 0);
    ck_assert_ptr_eq(ht->string_copy_fn, string_copy_fn);
    ck_assert_ptr_eq(ht->hash_fn, hash_fn);

    ck_assert(compact_hash_table_destroy(ht));
}
END_TEST

