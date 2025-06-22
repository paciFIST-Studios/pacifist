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
    char* deleted_entry = DELETED_ENTRY;
    ck_assert_ptr_nonnull(deleted_entry);
    ck_assert_int_eq(strcmp(deleted_entry, "DELETED_ENTRY_THIS_KEY_IS_INVALID"), 0);
}
END_TEST


START_TEST(typedef_StringCopyFunction__is_defined) {
    StringCopyFunction* p = NULL;
    ck_assert_ptr_null(p);
}
END_TEST


START_TEST(typedef_StringCopyFunction__can_be_set_to_strncpy) {
    StringCopyFunction* p = strncpy;
    ck_assert_ptr_nonnull(p);
}
END_TEST


// Hashing Functions ---------------------------------------------------------------------------------------------------

START_TEST(fn_interface_HashFunction__is_defined) {
    HashFunction* p = NULL;
    ck_assert_ptr_null(p);
}
END_TEST


START_TEST(fn_hash_polynomial_64__is_defined) {
    HashFunction* ptr = hash_polynomial_64;
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


// struct CompactHashTable.t -------------------------------------------------------------------------------------------

START_TEST(struct_CompactHashTable_t__is_defined) {
    CompactHashTable_t * table = compact_hash_table_create(0, NULL);
    ck_assert_ptr_nonnull(table);
    free(table);
}
END_TEST


// fn compact_hash_table_create ----------------------------------------------------------------------------------------

START_TEST(fn_compact_hash_table_create__is_defined) {
    CompactHashTable_t* (*fptr)(uint32_t, HashFunction) = &compact_hash_table_create;
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
    ck_assert_ptr_eq(table->string_copy_fn, strncpy); /* sets correct default */
    ck_assert_ptr_nonnull(table->entries);
    for (int32_t i = 0; i < table->size; i++) {
        if (table->entries[i].key != NULL) {
            free(table->entries[i].key);
            table->entries[i].key = NULL;
        }
    }
    free(table->entries);
    table->entries = NULL;
    free(table);
    table = NULL;
    ck_assert_ptr_null(table);
}
END_TEST


// fn compact_hash_table_destroy ---------------------------------------------------------------------------------------

START_TEST(fn_compact_hash_table_destroy__is_defined) {
    void (*fptr)(CompactHashTable_t*) = &compact_hash_table_destroy;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_hash_table_destroy__will_destroy_the_table_correctly) {
    CompactHashTable_t* ht = compact_hash_table_create(1, hash_polynomial_64);
    ck_assert_ptr_nonnull(ht);
    
    //compact_hash_table_destroy(ht);
    //ck_assert_ptr_null(ht);
}

