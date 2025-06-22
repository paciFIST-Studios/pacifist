// paciFIST studios. 2025. MIT License.

/**
 * The CompactHashTable_t prioritizes memory over speed. It uses
 * less memory than the QuickHashTable_t, but it is slower
 */

#ifndef COMPACT_HASH_TABLE_H
#define COMPACT_HASH_TABLE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ---------------------------------------------------------------------------------------------------------------------
// Defines
// ---------------------------------------------------------------------------------------------------------------------


// The deleted entry key is used to indicate that an array element is empty, in a way which
// will allow for successful retrieval, of a value which collided with this position, and was
// pleased elsewhere during a roll-over operation.
//
// You are not allowed to use this value as a key for anything stored in the hash table
#define DELETED_ENTRY "DELETED_ENTRY_THIS_KEY_IS_INVALID"
#define DELETED_ENTRY_LEN 34



// fwd declaration for string copy fn 
typedef char* (StringCopyFunction)(char *, const char *, size_t);


// ---------------------------------------------------------------------------------------------------------------------
// Hashing Functions
// ---------------------------------------------------------------------------------------------------------------------

// interface for hash-functions
typedef uint64_t (HashFunction)(char const *, size_t);


// Polynomial Rolling Hash
static uint64_t hash_polynomial_64(char const * key, size_t table_size){
    if(key == NULL || table_size == 0){
        return UINT64_MAX;
    }

    uint64_t const prime = 27644437;
    uint64_t power = 1;
    uint64_t hash = 0;

    for(uint64_t i = 0; i < strlen(key); i++){
        hash = (hash + ((key[i] - 'a' + 1) * power)) % table_size;
        power = (power * prime) % table_size;
    }

    return hash;
}


// Fowler-Noll-Vo hash
static uint64_t hash_fnv1a_64(char const *key, size_t table_size){
    if(key == NULL || table_size == 0){
        return UINT64_MAX;
    }

    uint64_t const OFFSET_BASIS = 2166136261U;
    uint64_t const HASH_PRIME = 16777619U;
    uint64_t hash = OFFSET_BASIS;
    
    for(uint64_t i = 0; i < strlen(key); i++){
        hash ^= (uint64_t)key[i];
        hash *= HASH_PRIME;
    }

    return hash;
}



// ---------------------------------------------------------------------------------------------------------------------
//    Data Structures
// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Do Not Use--for internal use by CompactHashTable only.
 */
typedef struct CompactHashTableEntry_t {
    // the key used to look up this entry
    char * key;
    // length of the lookup key
    size_t key_len;
    
    // the value stored in this entry
    void * value;
} CompactHashTableEntry_t;


/**
 * @brief CompactHashTable_t prioritizes small footprint.
 */
typedef struct _CompactHashTable_t {
    // how many elements can fit in this table
    uint32_t size; 
    // how many elements are in this table now
    uint32_t used;
    // the fn to use to has new keys
    HashFunction * hash_fn;
    // the fn to use to duplicate a string
    StringCopyFunction * string_copy_fn;
    // ptr to the array of entries
    CompactHashTableEntry_t * entries;
} CompactHashTable_t;



// ---------------------------------------------------------------------------------------------------------------------
//    Functions
// ---------------------------------------------------------------------------------------------------------------------


/**
 * @brief Creates and returns a CompactHashTable.
 *
 * @param size             the number of elements to allocate for this hash table
 * @param HashFunction*    a user-supplied hashing fn
 *
 * @return                 Null, on error, otherwise, an initialized CompactHashTable
 */
static CompactHashTable_t * compact_hash_table_create(uint32_t size, HashFunction * hf) {
    size_t const total_allocation = sizeof(CompactHashTable_t) + (sizeof(CompactHashTableEntry_t) * size);

    CompactHashTable_t * table = malloc(total_allocation);
    if (table == NULL) {
        // error, could not allocate memory for hash table
        return NULL;
    }

    table->size = size;
    table->used = 0;
    table->hash_fn = hf;

    // the entries start in the next byte after the table itself
    table->entries = (CompactHashTableEntry_t*)((void*)table + sizeof(CompactHashTable_t));

    // in the future, we might want to provide support for the user to add their own allocator,
    // so they can keep 100% of the memory used within the confines of their program
    table->string_copy_fn = strncpy;

    return table;
}


static void compact_hash_table_destroy(CompactHashTable_t * ht){
    if (ht == NULL){
        // error, tried to destory null
        return;
    }
   
    if(ht->entries != NULL) {
        int32_t const size = ht->size;
        for (int32_t i = 0; i < size; i++) {
            if (ht->entries[i].key != NULL) {
                free(ht->entries[i].key);
                ht->entries[i].key = NULL;
            }
        }
        free(ht->entries);
        ht->entries = NULL;
    } else {
        // warning, this table has no entries    
    }
    free(ht);
    ht = NULL;
}

//void compact_hash_table_print(CompactHashTable_t * ht){
//}
//
//void compact_hash_table_write_metadata(CompactHashTable_t * ht, FILE * file){
//}

static char* compact_hash_table_insert(CompactHashTable_t * ht, char const * key, size_t key_len, void * value) {
    if (ht == NULL) {
        // error, no ptr to hash table
        return NULL;
    }
    if (key == NULL) {
        // error, no ptr to key
        return NULL;
    } 
    if (key_len == 0) {
        // error, invalid key
        return NULL;
    } 
    if (value == NULL) {
        // error, cannot store null    
        return NULL;
    }
    if (strncmp(key, DELETED_ENTRY, DELETED_ENTRY_LEN) == 0) {
        // error, you can't use the deleted entry key to store anything
        return NULL;
    }

    // check for hash table resize here


    // the hashed value of this key
    uint64_t const hash = ht->hash_fn(key, key_len);
    // the *starting* index, for this hashed key.  
    // If this position is full, the index will change
    size_t index = hash % ht->size;

    size_t const start_index = index;
    bool has_looped = false;

    CompactHashTableEntry_t * entries = ht->entries;

    // if this insertion is in the table, we should update it
    while (entries[index].key != NULL && strncmp(entries[index].key, DELETED_ENTRY, DELETED_ENTRY_LEN) != 0){
        if (strncmp(entries[index].key, key, key_len) == 0){
           entries[index].value = value;
            return entries[index].key; 
        }

        // increment the index so we can keep looping
        ++index;
        
        // wrap the index so it stays inside the backing table's array
        if (index >= ht->size) {
            index = 0;
            has_looped = true;
        }
       
        if(has_looped && index == start_index){
            // error, the table is 100% full, but has not been resized
            return NULL;
        }
    }

    // we know we don't have this key, b/c we always start checking at the appropriate hash-index

    char * key_copy = strdup(key);
    if (key_copy == NULL) {
        // error, could not duplicate key
        return NULL;
    } 

    ht->used++;

    entries[index].key = key_copy;
    entries[index].value = value;

    return key_copy;
}

//void* compact_hash_table_lookup(CompactHashTable_t * ht, char const * key, size_t key_len){
//    return NULL;
//}
//
//bool compact_hash_table_remove(CompactHashTable_t * ht, char const * key, size_t key_len){
//    return false;
//}

#endif //COMPACT_HASH_TABLE_H
