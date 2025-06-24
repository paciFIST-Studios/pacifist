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


#define ERROR_COULD_NOT_ALLOCATE_MEMORY_FOR_HASH_TABLE "[CompactHashTable.h][error]: could not allocate memory for hash table"
#define ERROR_NO_PTR_TO_HASH_TABLE "[CompactHashTable.h][error]: no ptr to hash table"
#define ERROR_NO_PTR_TO_KEY "[CompactHashTable.h][error]: no ptr to key"
#define ERROR_INVALID_KEY "[CompactHashTable.h][error]: invalid key"
#define ERROR_CANNOT_STORE_NULL "[CompactHashTable.h][error]: cannot store NULL"
#define ERROR_CANT_USE_DELETED_ENTRY_AS_KEY "[CompactHashTable.h][error]: can't use deleted entry as key"
#define ERROR_TABLE_IS_ENTIRELY_FULL_BUT_HAS_NOT_BEEN_RESIZED "[CompactHashTable.h][error]: table is 100% full, but has not been resized.  Cannot insert key"
#define ERROR_COULD_NOT_DUPLICATE_KEY_STRING "[CompactHashTable.h][errror]: could not duplicate key string"


/*
 * @brief Do Not Use.  Call get_error_message to read messages stored to this ptr.
 */
static char * _global_error_message;

static char* get_error_message() {
   return _global_error_message; 
}


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

    // if true, hash table will automatically resize itself, when its
    // use hits a certain fill%.  Normally, for CompactHashTable,
    // this is set to false, and the user is asked to manually resize
    // the hash table with the  compact_hash_table_resize fn
    bool allow_auto_resize;
    float auto_resize_percent;
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
    _global_error_message = NULL;
    
    size_t const total_allocation = sizeof(CompactHashTable_t) + (sizeof(CompactHashTableEntry_t) * size);

    CompactHashTable_t * table = calloc(total_allocation, 1);
    if (table == NULL) {
        // error, could not allocate memory for hash table
        _global_error_message = ERROR_COULD_NOT_ALLOCATE_MEMORY_FOR_HASH_TABLE;
        return NULL;
    }

    table->size = size;
    table->used = 0;
    table->hash_fn = hf;

    // the entries start in the next byte after the table itself
    table->entries = (CompactHashTableEntry_t*)((void*)table + sizeof(CompactHashTable_t));
    
    // initialize all of these to their "zero" settings
    for (int32_t i = 0; i < size; i++) {
        table->entries[i].key = NULL;
        table->entries[i].key_len = 0;
        table->entries[i].value = NULL;
    }
    
    // in the future, we might want to provide support for the user to add their own allocator,
    // so they can keep 100% of the memory used within the confines of their program
    table->string_copy_fn = strndup;

    table->allow_auto_resize = false;
    table->auto_resize_percent = 1.0f;
    
    return table;
}


static bool compact_hash_table_destroy(CompactHashTable_t * ht){
    _global_error_message = NULL;
    
    if (ht == NULL){
        // error, tried to destroy null
        return false;
    }

    // iterate the entries in the entries array 
    if(ht->entries != NULL) {
        int32_t const size = ht->size;
        for (int32_t i = 0; i < size; i++) {
            if (ht->entries[i].key != NULL) {
                free(ht->entries[i].key);
                ht->entries[i].key = NULL;
                ht->entries[i].key_len = 0;
                ht->entries[i].value = NULL;
            }
        }
        // these entries are the same allocation as the table itself,
        // so we can't free them here, or it'll segfault
        //free((void*)ht->entries);

        // we don't have to zero these out, but let's do, so
        // we remember what's in there
        ht->entries = NULL;
        ht->used = 0;
        ht->size = 0;
        
        // we're not freeing these, they're in static memory
        ht->hash_fn = NULL;
        ht->string_copy_fn = NULL;
        
    } else {
        // warning, this table has no entries    
    }

    // freeing the table will also free the entries
    free(ht);
    ht = NULL;

    return true;
}

//void compact_hash_table_print(CompactHashTable_t * ht){
//}
//
//void compact_hash_table_write_metadata(CompactHashTable_t * ht, FILE * file){
//}




static char* compact_hash_table_insert(CompactHashTable_t * ht, char const * key, size_t key_len, void * value) {
    _global_error_message = NULL;
    
    if (ht == NULL) {
        // error, no ptr to hash table
        _global_error_message = ERROR_NO_PTR_TO_HASH_TABLE;
        return NULL;
    }
    if (key == NULL) {
        // error, no ptr to key
        _global_error_message = ERROR_NO_PTR_TO_KEY;
        return NULL;
    } 
    if (key_len == 0) {
        // error, invalid key
        _global_error_message = ERROR_INVALID_KEY;
        return NULL;
    } 
    if (value == NULL) {
        // error, cannot store null
        _global_error_message = ERROR_CANNOT_STORE_NULL;
        return NULL;
    }
    if (strncmp(key, DELETED_ENTRY, DELETED_ENTRY_LEN) == 0) {
        // error, you can't use the deleted entry key to store anything
        _global_error_message = ERROR_CANT_USE_DELETED_ENTRY_AS_KEY;
        return NULL;
    }

    
    if (ht->allow_auto_resize) {
        // do resize
        /* For the CompactHashTable, make it so it can optionally resize at a
         * specific use-%, and otherwise, it doesn't resize ever.  But,
         * do include a resize fn, so the user can manually resize it when
         * they like. (It just won't "help")
         */
    }

    
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
            _global_error_message = ERROR_TABLE_IS_ENTIRELY_FULL_BUT_HAS_NOT_BEEN_RESIZED;
            return NULL;
        }
    }

    // we know we don't have this key, b/c we always start checking at the appropriate hash-index

    char * key_copy = ht->string_copy_fn(key, key_len);
    if (key_copy == NULL) {
        // error, could not duplicate key
        _global_error_message = ERROR_COULD_NOT_DUPLICATE_KEY_STRING;
        return NULL;
    } 


    entries[index].key = key_copy;
    entries[index].key_len = key_len;
    entries[index].value = value;

    ht->used++;

    return key_copy;
}

//void* compact_hash_table_lookup(CompactHashTable_t * ht, char const * key, size_t key_len){
//    return NULL;
//}
//
//bool compact_hash_table_remove(CompactHashTable_t * ht, char const * key, size_t key_len){
//    return false;
//}

static CompactHashTable_t* compact_hash_table_resize(CompactHashTable_t* ht, float increase_factor) {
    if (ht == NULL) {
        // error, null arg
        return NULL;
    }

    if (increase_factor < 0.0f) {
        // warn, use destroy fn to destroy table
        return NULL;
    }

    if (increase_factor < 1.0f) {
        // TODO: figure out what to do in this case
        //  just shrink down until it's 100% full, and do nothing with existing entries?
        return NULL;
    }
    
    if (ht->entries == NULL) {
        // warn, invalid entries ptr in table
        return NULL;
    }

    // prepare to copy
    size_t const old_table_size = ht->size;
    size_t const new_table_size = ht->size * increase_factor + 1;

    // creates a new hash table of the correct size, but it's empty
    CompactHashTable_t* new_ht = compact_hash_table_create(new_table_size, ht->hash_fn);

    new_ht->allow_auto_resize = ht->allow_auto_resize;
    new_ht->auto_resize_percent = ht->auto_resize_percent;

    // iterate all the existing entries
    for (int32_t i = 0; i < old_table_size; i++) {
        CompactHashTableEntry_t* entry = &ht->entries[i];
        // skip empty entries, but also skip deleted entries, b/c those are only needed to re-find
        // the roll-over position of something that was inserted with a collision, but then removed.
        // we'll re-create equivalent conditions by inserting the keys and values again, simply
        // by using the insert fn
        if (entry->key != NULL && !is_deleted_entry_key(entry->key)) {
            compact_hash_table_insert(new_ht, entry->key, entry->key_len, entry->value);
        }
    }

    compact_hash_table_destroy(ht);

    return new_ht;
}


#endif //COMPACT_HASH_TABLE_H
