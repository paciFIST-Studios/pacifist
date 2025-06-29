// paciFIST studios. 2025. MIT License.

/**
 * The CompactHashTable_t prioritizes memory over speed. It uses
 * less memory than the QuickHashTable_t, but it is slower
 */

#ifndef COMPACT_HASH_TABLE_H
#define COMPACT_HASH_TABLE_H

// stdlib
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// framework
// project


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


#define ERROR_COULD_NOT_ALLOCATE_MEMORY_FOR_HASH_TABLE "[CompactHashTable.h][error]: could not allocate memory for hash table"
#define ERROR_NO_PTR_TO_HASH_TABLE "[CompactHashTable.h][error]: no ptr to hash table"
#define ERROR_NO_PTR_TO_KEY "[CompactHashTable.h][error]: no ptr to key"
#define ERROR_INVALID_KEY "[CompactHashTable.h][error]: invalid key"
#define ERROR_CANNOT_STORE_NULL "[CompactHashTable.h][error]: cannot store NULL"
#define ERROR_CANT_USE_DELETED_ENTRY_AS_KEY "[CompactHashTable.h][error]: can't use deleted entry as key"
#define ERROR_TABLE_IS_ENTIRELY_FULL_BUT_HAS_NOT_BEEN_RESIZED "[CompactHashTable.h][error]: table is 100% full, but has not been resized.  Cannot insert key"
#define ERROR_COULD_NOT_DUPLICATE_KEY_STRING "[CompactHashTable.h][errror]: could not duplicate key string"


/**
 * Returns an internally set error message, giving better detail when an error has occurred
 * during the operation of this a CompactHashTable_t
 * @return 
 */
char* compact_hash_table_get_error_message();

// fwd declaration for string copy fn 
typedef char* (StringCopyFunction)(const char * src, size_t len);


// ---------------------------------------------------------------------------------------------------------------------
// Helper Functions
// ---------------------------------------------------------------------------------------------------------------------

static inline bool is_deleted_entry_key(char const * key) {
    if  (key == NULL) {
        return false;
    }
    
    return strncmp(key, DELETED_ENTRY, DELETED_ENTRY_LEN) == 0;
}


// ---------------------------------------------------------------------------------------------------------------------
// Hashing Functions
// ---------------------------------------------------------------------------------------------------------------------

// interface for hash-functions
typedef uint64_t (HashFunction)(char const *, size_t);


// Polynomial Rolling Hash
/**
 * 
 * @param key 
 * @param table_size 
 * @return 
 */
static uint64_t hash_polynomial_64(char const * key, size_t table_size){
    if(key == NULL){
        // error cannot hash null key
        return UINT64_MAX;
    }
    if (table_size == 0) {
        // error table size is zero
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
/**
 * 
 * @param key 
 * @param table_size 
 * @return 
 */
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
typedef struct CompactHashTable_t {
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
 * @param hf                a user-supplied hashing fn
 *
 * @return                 Null, on error, otherwise, an initialized CompactHashTable
 */
CompactHashTable_t * compact_hash_table_create(uint32_t size, HashFunction * hf);


/**
 * @brief Destroys the supplied CompactHashTable_t and deallocates all memory used by it.
 * 
 * @param ht 
 * @return 
 */
bool compact_hash_table_destroy(CompactHashTable_t * ht);


void compact_hash_table_print(CompactHashTable_t * ht);


//
//void compact_hash_table_write_metadata(CompactHashTable_t * ht, FILE * file){
//}



/**
 * @brief Inserts the supplied value, using the supplied key, into the supplied table
 *
 * @param ht 
 * @param key 
 * @param key_len 
 * @param value   
 *
 * @returns char const *
 */
char const * compact_hash_table_insert(CompactHashTable_t * ht, char const * key, size_t const key_len, void * value);


/**
 * @brief Looks up an entry in a CompactHashTable_t, and returns NULL, unless the entry is there.
 *
 * @param ht                        the hash table to search
 * @param key                       the key to look for in the hash map
 * @param key_len                   the length of the search key
 *
 * @returns NULL, on error, or the data associated with this key.
 */
void* compact_hash_table_lookup(CompactHashTable_t * ht, char const * key, size_t const key_len);




//
//bool compact_hash_table_remove(CompactHashTable_t * ht, char const * key, size_t key_len){
//    return false;
//}


/**
 * 
 * @param ht 
 * @param increase_factor 
 * @return 
 */
CompactHashTable_t* compact_hash_table_resize(CompactHashTable_t* ht, float increase_factor);


#endif //COMPACT_HASH_TABLE_H
