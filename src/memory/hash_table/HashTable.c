// paciFIST studios. 2025. MIT License

// header
#include "HashTable.h"

// stdlib
#include <stdlib.h>
#include <string.h>
// framework
// project


HashTable_t* hash_table_create(void) {
    HashTable_t* table = malloc(sizeof(HashTable_t));
    if (table == NULL){
        // TODO: write error for could not allocate table
        return NULL;
    }

    table->used = 0;
    table->capacity = HASH_TABLE_DEFAULT_CAPACITY;

    table->_entries = calloc(table->capacity, sizeof(HashTableEntry_t));
    if (table->_entries == NULL) {
        // TODO: write error for could not allocate entries
        free(table);
        return NULL;
    }
    
    return table;
}


void hash_table_destroy(HashTable_t* table) {
    if (table == NULL) {
        return;
    }
    
    // free the memory allocated for the keys
    for (size_t i = 0; i < table->used; i++) {
        free((void*)table->_entries[i].key);
    }

    // as long as we never pull entries from outside the pre-defined entries block, this is okay
    free(table->_entries);
    free(table);
}





/**
 * This algorithm is neither randomized, nor cryptographic, so it's not secured
 * against a slowdown attack, where a user provides keys with notably-many
 * collisions. Okay for use offline, in a single player game.
 */
// https://benhoyt.com/writings/hash-table-in-c/
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// this is a 64-bit hash, might need an alternative to support 32 bit machines
static uint64 hash_key_64(char const * key) {
    uint64 hash = FNV_OFFSET;
    for (char const * p = key; *p; p++) {
        hash ^= (uint64)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}


void* hash_table_lookup(HashTable_t const * table, char const * key) {
    uint64 const hash = hash_key_64(key);
    size_t const key_len = strlen(key);
    
    size_t index = (size_t)(hash & (uint64)(table->capacity - 1));

    // loop all of the entries
    while (table->_entries[index].key != NULL) {
        if (strncmp(key, table->_entries[index].key, key_len) == 0) {
            // this is the key we're looking for
            return table->_entries[index].value;
        }

        // we didn't find the key at this index, so look at the next index
        index++;
        
        // wrap index when we get to the end of the array
        if (index >= table->capacity) {
            index = 0;
        }
    }
    
    return NULL;
}


char const * _hash_table_insert_new_entry(HashTableEntry_t* entries, size_t capacity, char const * key, void* value, size_t* pUsed) {
    if (entries == NULL) {
        // error, cannot operate on null array
        return NULL;
    }
    if (capacity == 0) {
        // error, cannot operate on array with no capacity
        return NULL;
    }
    if (key == NULL) {
        // error, cannot insert a value without a key
        return NULL;
    }
    if (value == NULL) {
        // error, cannot insert a null value
        return NULL;
    }

    // if pUsed == NULL <-- this is okay, we do this when expanding the hash table

    uint64 const hash = hash_key_64(key);
    size_t const key_len = strlen(key);
    size_t index = (size_t)(hash & (uint64)(capacity - 1));

    bool32 has_looped = FALSE;
    
    // loop the backing array, until we find a spot
    while (entries[index].key != NULL) {
        if (strncmp(key, entries[index].key, key_len) == 0) {
            // this key already exists, but the user is allowed to update the value
            entries[index].value = value;
            return entries[index].key;
        }
        // key wasn't in this slot
        index++;
        
        if (index >= capacity) {
            if (has_looped) {
                // error, has looped through available entries and all are full
                return NULL;
            }
            
            // loop around to check again
            index = 0;
            has_looped = TRUE;
        }
    } // loop naturally breaks on an empty entry

    char const * new_key = NULL;
    if (pUsed != NULL) {
        new_key = strndup(key, key_len);
        if (new_key == NULL) {
            // error, could not allocate memory for new key
            return NULL;
        }
        (*pUsed)++;
    }

    entries[index].key = new_key;
    entries[index].value = value; 

    return new_key;
}


char const * hash_table_insert(HashTable_t * table, char const * key, void* value) {
    if (table == NULL) {
        // error: cannot operate on null table
        return NULL;
    }

    if (value == NULL) {
        // error: cannot operate with null value
        return NULL;
    }

    if (key == NULL) {
        // error: cannot operate with null key
        return NULL;
    }

    if (table->used >= table->capacity/2) {
        if (!_hash_table_expand(table)) {
            return NULL;
        }
    } 

    return _hash_table_insert_new_entry(table->_entries, table->capacity, key, value, &table->used);
}


bool32 _hash_table_expand(HashTable_t* table) {

    size_t const new_capacity = table -> capacity * HASH_TABLE_WHEN_RESIZING_INCREASE_SIZE_BY_FACTOR;
    if (new_capacity < table->capacity) {
        // error: this would overflow memory 
        return FALSE;
    }

    HashTableEntry_t* new_entries = calloc(new_capacity, sizeof(HashTableEntry_t));
    if (new_entries == NULL) {
        // error: could not allocate new entries to resize
        return FALSE;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        HashTableEntry_t const entry = table->_entries[i];
        if (entry.key != NULL) {
            _hash_table_insert_new_entry(new_entries, new_capacity, entry.key, entry.value, NULL);
        }
    }

    free(table->_entries);
    table->_entries = new_entries;
    table->capacity = new_capacity;
    
    return TRUE;
}

size_t hash_table_count(HashTable_t const * table) {
    return 0;
}





//HashTableIterator_t hash_table_create_iterator(HashTable_t * table) {
//    return HashTableIterator_t{0};
//}

bool32 hash_table_next(HashTableIterator_t * iterator) {
    return FALSE;
}