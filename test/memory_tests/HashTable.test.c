// paciFIST studios. 2025. MIT License

// include
#include "check.h"
#include "../../src/memory/hash_table/HashTable.h"


// stdlib
#include <stdlib.h>
// framework
// project


// struct HashTableEntry_t ---------------------------------------------------------------------------------------------

START_TEST(struct_HashTableEntry_t__is_defined) {
    HashTableEntry_t entry;
    ck_assert_ptr_nonnull(&entry);
}
END_TEST

START_TEST(struct_HashTableEntry_t__has_correct_properties) {
    HashTableEntry_t entry;
    ck_assert_ptr_nonnull(&entry);

    entry.key = NULL;
    entry.value = NULL;
    entry.pNext = NULL;

    ck_assert_ptr_null(entry.key);
    ck_assert_ptr_null(entry.value);
    ck_assert_ptr_null(entry.pNext);
}
END_TEST

START_TEST(struct_HashTableEntry_t__has_correct_size) {
    HashTableEntry_t entry;
    ck_assert_int_eq(sizeof(entry), 24);
}
END_TEST



// struct HashTable_t --------------------------------------------------------------------------------------------------

START_TEST(struct_HashTable_t__is_defined) {
    HashTable_t hash_table;
    ck_assert_ptr_nonnull(&hash_table);
}
END_TEST

START_TEST(struct_HashTable_t__has_correct_properties) {
    HashTable_t hash_table;
    ck_assert_ptr_nonnull(&hash_table);

    hash_table._entries = NULL;
    hash_table.capacity = 0;
    hash_table.used = 0;

    ck_assert_ptr_null(hash_table._entries);
    ck_assert_int_eq(hash_table.capacity, 0);
    ck_assert_int_eq(hash_table.used, 0);
}
END_TEST

START_TEST(struct_HashTable_t__has_correct_size) {
    HashTable_t hash_table;
    ck_assert_int_eq(sizeof(hash_table), 56);
}
END_TEST



// fn hash_table_create ------------------------------------------------------------------------------------------------

START_TEST(fn_hash_table_create__is_defined) {
    HashTable_t*(*fptr)() = &hash_table_create;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_hash_table_create__returns_valid_hash_table) {
    HashTable_t* ht = hash_table_create();
    ck_assert_ptr_nonnull(ht);
    ck_assert_int_eq(ht->capacity, HASH_TABLE_DEFAULT_CAPACITY);
    ck_assert_int_eq(ht->used, 0);
    // ptr size
    ck_assert_int_eq(sizeof(ht), 8);
    // hash table size
    ck_assert_int_eq(sizeof(*ht), 24);
    // elements ptr size
    ck_assert_int_eq(sizeof(ht->_entries), 8);

    free(ht);
}
END_TEST

START_TEST(fn_hash_table_create__returns_null__for_nomem_cannot_create_table) {
}
END_TEST

START_TEST(fn_hash_table_create__returns_null__for_nomem_cannot_create_elements) {
}
END_TEST

// fn hash_table_destroy -----------------------------------------------------------------------------------------------

START_TEST(fn_hash_table_destroy__is_defined) {
    void(*fptr)(HashTable_t* hash_table) = &hash_table_destroy;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_hash_table_destroy__will_destory_the_table_and_entries) {
    HashTable_t* table = hash_table_create();
    ck_assert_ptr_nonnull(table);
    
    size_t const capacity = table->capacity;
    
    hash_table_destroy(table);

    // todo: find a better way of visualizing this
    ck_assert_int_ne(table->capacity, capacity);
}
END_TEST

// fn hash_table_expand ------------------------------------------------------------------------------------------------

START_TEST(fn_hash_table_expand__is_defined) {
    bool32(*fptr)(HashTable_t* hash_table) = &_hash_table_expand;
    ck_assert_ptr_nonnull(fptr);    
}
END_TEST



// fn hash_table_lookup ------------------------------------------------------------------------------------------------

START_TEST(fn_hash_table_lookup__is_defined) {
    void*(*fptr)(HashTable_t const * hash_table, char const * key) = &hash_table_lookup;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_hash_table_lookup__) {
    
}
END_TEST



// fn hash_table_insert ------------------------------------------------------------------------------------------------

START_TEST(fn_hash_table_insert__is_defined) {
    char const * (*fptr)(HashTable_t* hash_table, char const * key, void* value) = &hash_table_insert;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn__hash_table_insert_element__is_defined) {
}
END_TEST


// fn hash_table_count -------------------------------------------------------------------------------------------------

START_TEST(fn_hash_table_count__is_defined) {
    size_t(*fptr)(HashTable_t const * hash_table) = &hash_table_count;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// struct HashTableIterator_t ------------------------------------------------------------------------------------------

START_TEST(struct_HashTableIterator_t__is_defined) {
    HashTableIterator_t const iterator;
    ck_assert_ptr_nonnull(&iterator);
}
END_TEST


// fn hash_table_create_iterator ---------------------------------------------------------------------------------------

//START_TEST(fn_hash_table_create_iterator__is_defined) {
//    HashTableIterator_t (*fptr)(HashTable_t* hash_table) = &hash_table_create_iterator;
//    ck_assert_ptr_nonnull(fptr);
//}
//END_TEST

// fn hash_table_next --------------------------------------------------------------------------------------------------

START_TEST(fn_hash_table_next__is_defined) {
    bool32(*fptr)(HashTableIterator_t* iterator) = &hash_table_next;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

