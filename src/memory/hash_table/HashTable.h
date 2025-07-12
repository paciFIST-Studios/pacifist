// paciFIST studios. 2025. MIT License

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// stdlib
#include <stddef.h>
// framework
// project
#include "../../core/common.h"


/**
 * @brief A local alias for size_t, which HashTable_t uses.
 */
typedef size_t HashTableIndex_t;


#define HASH_TABLE_DEFAULT_CAPACITY 16
#define HASH_TABLE_WHEN_RESIZING_INCREASE_SIZE_BY_FACTOR 2


/**
 * @brief The struct for a single entry in the hash table. 
 */
typedef struct HashTableEntry_t {
    char const * key;
    void* value;

    struct HashTableEntry_t* pNext;
} HashTableEntry_t;


typedef struct HashTable_t {
    // array of entries in the hash table
    HashTableEntry_t* _entries;
    // length of the _entries array
    size_t capacity;
    // number of elements which have been inserted into the hash table
    size_t used;
    
} HashTable_t;


/**
 * @brief Allocates and returns a HashTable_t* .
 *
 * @return (HashTable_t*) pointer to a newly allocated hash table type
 */
HashTable_t* hash_table_create(void);


/**
 * @brief Destroys the supplied hash table
 *
 * @param table - a pointer to an in-memory hash table, which will be freed
 */
void hash_table_destroy(HashTable_t* table);

// this fn (and some others) should be static, maybe split it out to
// a utility file or something?  Right now, it's not static, so we can
// test it.  If we moved all of the header into a c file, we could
// still make it static, and still use it, but then there's no header
/**
 * @brief  Do not call this fn---it is an internal helper for HashTable.
 * 
 * @param table 
 * @return 
 */
bool32 _hash_table_expand(HashTable_t* table);



/**
 * @brief Looks for an element in the hash table, and returns its value, if found.
 *
 * 
 * @param table - pointer to an in-memory hash-table
 * @param key -  pointer to an in-memory, const string
 * 
 * @return - a void*, which the user will need to cast to the correct type 
 */
void* hash_table_lookup(HashTable_t const * table, char const * key);


/**
 * @brief Inserts a value into the hash table, and associates it with the given key.
 *
 * @param table - pointer to an in-memory hash-table
 * @param key - pointer to an in-memory, nul-terminated, const string
 * @param value - pointer to some non-null value
 * 
 * @return - the address of a copy of the supplied key, or null, if out of memory 
 */
char const * hash_table_insert(HashTable_t* table, char const * key, void* value);


/**
 * @brief  Do not call this fn---it is an internal helper for HashTable.
 * 
 * @param entries - non-null pointer to the HashTable_t's hash table entries array
 * @param capacity 
 * @param key 
 * @param value 
 * @param pUsed 
 * @return 
 */
char const * _hash_table_insert_new_entry(HashTableEntry_t* entries, size_t capacity, char const * key, void* value, size_t* pUsed);



/**
 * @brief Returns the number of items stored in the hash table.
 * 
 * @param table 
 * @return 
 */
size_t hash_table_count(HashTable_t const * table);


// create with hash_table_create_iterator
// use with hash_table_next
typedef struct HashTableIterator_t {
    char const * key;
    void * value;

    // 
    HashTable_t* _hash_table;
    HashTableIndex_t _index; 
    
} HashTableIterator_t;

/**
 * @brief Allocates and returns a new HashTableIterator_t.
 * 
 * @param table 
 * @return 
 */
HashTableIterator_t hash_table_create_iterator(HashTable_t * table);



/**
 * @brief Moves the iterator to the next item in the hash table, and updates the iterators key and value.
 * 
 * @param iterator 
 * @return - False, if there are no other entries, otherwise true. 
 */
bool32 hash_table_next(HashTableIterator_t* iterator);


#endif //HASH_TABLE_H
