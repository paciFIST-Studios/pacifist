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
#include "../../project_data_types.h"

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


#define ERROR_INVALID_TABLE_LENGTH "[CompactHashTable.h][error]: Cannot produce hash for zero length table"

#define ERROR_COULD_NOT_ALLOCATE_MEMORY_FOR_HASH_TABLE "[CompactHashTable.h][error]: could not allocate memory for hash table"
#define ERROR_CANNOT_DEALLOCATE_NULL_PTR_TO_HASH_TABLE "[CompactHashTable.h][error]: cannot deallocate null ptr to hash table"
#define ERROR_NULL_PTR_TO_HASH_TABLE "[CompactHashTable.h][error]: no ptr to hash table"
#define ERROR_NULL_PTR_TO_KEY "[CompactHashTable.h][error]: no ptr to key"
#define ERROR_INVALID_KEY_LENGTH "[CompactHashTable.h][error]: invalid key length"
#define ERROR_UNDEFINED_DATA_TYPE "[CompactHashTable.h][error]: undefined type for stored value"
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
typedef char* (StringCopyFunction_t)(const char * src, size_t len);


// ---------------------------------------------------------------------------------------------------------------------
// Helper Functions
// ---------------------------------------------------------------------------------------------------------------------

__attribute__((unused))
static bool is_deleted_entry_key(char const * key) {
    if  (key == NULL) {
        return false;
    }
    
    return strncmp(key, DELETED_ENTRY, DELETED_ENTRY_LEN) == 0;
}


// ---------------------------------------------------------------------------------------------------------------------
// Hashing Functions
// ---------------------------------------------------------------------------------------------------------------------

// interface for hash-functions
typedef uint64_t (HashFunction_t)(char const *, size_t const);


// Polynomial Rolling Hash
/**
 * @brief Hashes the supplied key into a uint64_t
 * 
 * @param key 
 * @param table_size 
 * @return 
 */
uint64_t hash_polynomial_64(char const * key, size_t const table_size);


// Fowler-Noll-Vo hash
/**
 * @brief Hashes the supplied key into a uint64_t
 * 
 * @param key 
 * @param table_size 
 * @return 
 */
uint64_t hash_fnv1a_64(char const *key, size_t const table_size);



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

    // the type of this value, implying how it should be deserialized
    EProjectDataTypes_t value_type;
    
    // the value stored in this entry
    void * value;
} CompactHashTableEntry_t;


/**
 * @brief CompactHashTable_t prioritizes small footprint.
 */
typedef struct PFCompactHashTable_t {
    // how many elements can fit in this table
    uint32_t size; 
    // how many elements are in this table now
    uint32_t used;
    // the fn to use to has new keys
    HashFunction_t * hash_fn;
    // the fn to use to duplicate a string
    StringCopyFunction_t * string_copy_fn;
    // ptr to the array of entries
    CompactHashTableEntry_t * entries;
} PFCompactHashTable_t;



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
PFCompactHashTable_t * compact_hash_table_create(
    uint32_t const size,
    HashFunction_t * hf);


/**
 * @brief Destroys the supplied CompactHashTable_t and deallocates all memory used by it.
 * 
 * @param ht 
 * @return 
 */
bool compact_hash_table_destroy(PFCompactHashTable_t * ht);


/**
 * @brief Prints data and some contents of the CompactHashTable to stdout
 * 
 * @param ht 
 */
void compact_hash_table_print(PFCompactHashTable_t * ht);


//
//void compact_hash_table_write_metadata(CompactHashTable_t * ht, FILE * file){
//}


/**
 * @brief Inserts the supplied value, using the supplied key, into the supplied table
 *
 * @param ht 
 * @param key 
 * @param key_len
 * @param value_type
 * @param value   
 *
 * @returns char const *
 */
char const * compact_hash_table_insert(
    PFCompactHashTable_t * ht,
    char const * key,
    size_t const key_len,
    EProjectDataTypes_t value_type,
    void * value);


/**
 * @brief Looks up a value in a CompactHashTable_t, and returns NULL, unless the value is there.
 *
 * @param ht                        the hash table to search
 * @param key                       the key to look for in the hash map
 * @param key_len                   the length of the search key
 *
 * @returns NULL on error, or the data associated with this key.
 */
void* compact_hash_table_lookup(
    PFCompactHashTable_t * ht,
    char const * key,
    size_t const key_len);


/**
 * @brief Looks up an entry in a CompactHashTable_t, and returns NULL, unless the entry is there.
 *
 *  Returning the entry means a user can determine the data_type of the value, because this
 *  information exists in the entry
 * 
 * @param ht 
 * @param key 
 * @param key_len 
 * @return NULL on error, or the data associated with this entry 
 */
CompactHashTableEntry_t const * compact_hash_table_lookup_entry(
    PFCompactHashTable_t * ht,
    char const * key,
    size_t const key_len
);


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
PFCompactHashTable_t* compact_hash_table_resize(
    PFCompactHashTable_t* ht,
    float increase_factor);


#endif //COMPACT_HASH_TABLE_H
