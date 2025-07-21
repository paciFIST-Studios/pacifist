// paciFIST studios. 2025. MIT License

#include "CompactHashTable.h"

// ---------------------------------------------------------------------------------------------------------------------
// Utility Functions
// ---------------------------------------------------------------------------------------------------------------------


/*
 * @brief Do Not Use.  Call get_error_message to read messages stored to this ptr.
 */
static char * compact_hash_table_global_error_message;

char* compact_hash_table_get_error_message() {
    return compact_hash_table_global_error_message; 
}

// ---------------------------------------------------------------------------------------------------------------------
// Helper Functions
// ---------------------------------------------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------------------------------------------
// Hashing Functions
// ---------------------------------------------------------------------------------------------------------------------

uint64_t hash_polynomial_64(char const * key, size_t const table_size) {
    compact_hash_table_global_error_message = NULL;
    
    if(key == NULL){
        compact_hash_table_global_error_message = ERROR_NULL_PTR_TO_KEY;
        return UINT64_MAX;
    }
    if (table_size == 0) {
        compact_hash_table_global_error_message = ERROR_INVALID_TABLE_LENGTH;
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


uint64_t hash_fnv1a_64(char const *key, size_t const table_size){
    compact_hash_table_global_error_message = NULL;

    if(key == NULL){
        compact_hash_table_global_error_message = ERROR_NULL_PTR_TO_KEY;
        return UINT64_MAX;
    }
    
    if (table_size == 0){
        compact_hash_table_global_error_message = ERROR_INVALID_TABLE_LENGTH;
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
// CompactHashTable Functions
// ---------------------------------------------------------------------------------------------------------------------

CompactHashTable_t* compact_hash_table_create(uint32_t const size, HashFunction_t * hf) {
    compact_hash_table_global_error_message = NULL;  
    size_t const hash_table_size = sizeof(CompactHashTable_t);
    size_t const hash_table_entry_size = sizeof(CompactHashTableEntry_t);
    size_t const total_allocation = hash_table_size + (hash_table_entry_size * size);

    CompactHashTable_t * table = calloc(total_allocation, 1);
    if (table == NULL) {
        compact_hash_table_global_error_message  = ERROR_COULD_NOT_ALLOCATE_MEMORY_FOR_HASH_TABLE;
        return NULL;
    }

    table->size = size;
    table->used = 0;
    table->hash_fn = hf;

    // the entries start in the next byte after the table itself
    table->entries = (CompactHashTableEntry_t*)(table + 1);
    
    // initialize all of these to their "zero" settings
    for (int32_t i = 0; i < size; i++) {
        table->entries[i].key = NULL;
        table->entries[i].key_len = 0;
        table->entries[i].value = NULL;
    }
    
    // in the future, we might want to provide support for the user to add their own allocator,
    // so they can keep 100% of the memory used within the confines of their program
    table->string_copy_fn = strndup;

    return table;
}



bool compact_hash_table_destroy(CompactHashTable_t* ht) {
    compact_hash_table_global_error_message = NULL;
    
    if (ht == NULL){
        compact_hash_table_global_error_message = ERROR_CANNOT_DEALLOCATE_NULL_PTR_TO_HASH_TABLE;
        return false;
    }

    // iterate the entries in the entries array 
    if(ht->entries != NULL) {
        uint32_t const size = ht->size;
        for (uint32_t i = 0; i < size; i++) {
            if (ht->entries[i].key != NULL) {
                // these keys are copied using strncpy, so they're
                // heap allocated, and will need to be freed individually
                if (ht->entries[i].key != NULL) {
                    free(ht->entries[i].key);
                }
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



static void compact_hash_table_print_value(EProjectDataTypes_t value_type, void* value) {
    printf("\t\tvalue_type: ");
    char const * message = "%s\n\t\tvalue:      \"%s\"\n";
    
    if (value_type  == EPDT__Undefined_t) {
        return;
    }

    if (value_type == EPDT__char_const_ptr_t) {
        printf(message, GLOBAL_TYPE_STRING__CHAR_CONST_PTR_T, (char const *)value);
    }
    if (value_type == EPDT__char_ptr_t) {
        printf(message, GLOBAL_TYPE_STRING__CHAR_PTR_T, (char*) value);
    }
}


void compact_hash_table_print(CompactHashTable_t* ht) {
    printf("[CompactHashTable_t] -- START\n");
    
    if (ht != NULL) {
        printf("\taddress: %p\n\tused:    %d\n\tmax:     %d\n\n", (void*)ht, ht->used, ht->size);
        
        if (ht->entries != NULL) {
            for (int32_t i = 0; i < ht->used; i++) {
                CompactHashTableEntry_t * entry = &ht->entries[i];
                printf("\t\tmap_index:  %d\n\t\tkey:        \"%s\"\n\t\tkey_len:    %ld\n\t\tvalue_addr: %p\n", i, entry->key, entry->key_len, entry->value);
                compact_hash_table_print_value(entry->value_type, entry->value);
                printf("\n");
            }
        }
    }
    else
    {
        printf("\t\tCompactHashTable_t * ht == NULL\n");       
    }
    
    printf("[CompactHashTable] -- END\n"); 
}



char const * compact_hash_table_insert(
    CompactHashTable_t* ht,
    char const * key,
    size_t const key_len,
    EProjectDataTypes_t const value_type,
    void * value)
{
    compact_hash_table_global_error_message = NULL;
    
    if (ht == NULL) {
        // error, no ptr to hash table
        compact_hash_table_global_error_message = ERROR_NULL_PTR_TO_HASH_TABLE;
        return NULL;
    }
    if (key == NULL) {
        // error, no ptr to key
        compact_hash_table_global_error_message = ERROR_NULL_PTR_TO_KEY;
        return NULL;
    } 
    if (key_len == 0) {
        compact_hash_table_global_error_message = ERROR_INVALID_KEY_LENGTH;
        return NULL;
    }
    if (value_type == EPDT__Undefined_t) {
        // error, undefined type
        compact_hash_table_global_error_message = ERROR_UNDEFINED_DATA_TYPE;
        return NULL;
    }
    if (value == NULL) {
        // error, cannot store null
        compact_hash_table_global_error_message = ERROR_CANNOT_STORE_NULL;
        return NULL;
    }
    if (strncmp(key, DELETED_ENTRY, DELETED_ENTRY_LEN) == 0) {
        // error, you can't use the deleted entry key to store anything
        compact_hash_table_global_error_message = ERROR_CANT_USE_DELETED_ENTRY_AS_KEY;
        return NULL;
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
    while (entries[index].key != NULL && !is_deleted_entry_key(entries[index].key)){
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
            compact_hash_table_global_error_message = ERROR_TABLE_IS_ENTIRELY_FULL_BUT_HAS_NOT_BEEN_RESIZED;
            return NULL;
        }
    }

    // we know we don't have this key, b/c we always start checking at the appropriate hash-index

    char * key_copy = ht->string_copy_fn(key, key_len);
    if (key_copy == NULL) {
        // error, could not duplicate key
        compact_hash_table_global_error_message = ERROR_COULD_NOT_DUPLICATE_KEY_STRING;
        return NULL;
    } 

    entries[index].key = key_copy;
    entries[index].key_len = key_len;
    entries[index].value_type = value_type;
    entries[index].value = value;

    ht->used++;

    return key_copy;
}

void* compact_hash_table_lookup(CompactHashTable_t * ht, char const * key, size_t const key_len){
    compact_hash_table_global_error_message = NULL;

    if (ht == NULL) {
        compact_hash_table_global_error_message = ERROR_NULL_PTR_TO_HASH_TABLE;
        return NULL;
    }
    if (key == NULL) {
        compact_hash_table_global_error_message = ERROR_NULL_PTR_TO_KEY;
        return NULL;
    }
    if (key_len == 0) {
        compact_hash_table_global_error_message = ERROR_INVALID_KEY_LENGTH;
        return NULL;
    }

    CompactHashTableEntry_t const * entry = compact_hash_table_lookup_entry(ht, key, key_len);
    if (entry == NULL) {
        return NULL;
    }
    
    return entry->value;
}


CompactHashTableEntry_t const * compact_hash_table_lookup_entry(CompactHashTable_t * ht, char const * key, size_t const key_len ) {
    compact_hash_table_global_error_message = NULL;

    if (ht == NULL) {
        // error, null arg
        compact_hash_table_global_error_message = ERROR_NULL_PTR_TO_HASH_TABLE;
        return NULL;
    }

    if (key == NULL) {
        // error, null arg
        compact_hash_table_global_error_message = ERROR_NULL_PTR_TO_KEY;
        return NULL;
    }

    if (key_len == 0) {
        compact_hash_table_global_error_message = ERROR_INVALID_KEY_LENGTH;
        return NULL;
    }

    size_t const index = ht->hash_fn(key, key_len) % ht->size;
    CompactHashTableEntry_t const * entries = ht->entries;
    while (entries[index].key != NULL && !is_deleted_entry_key(entries[index].key)) {
        if (strncmp(entries[index].key, key, key_len) == 0){
            return &entries[index]; 
        }
    } 
    
    return NULL;
}


CompactHashTable_t* compact_hash_table_resize(CompactHashTable_t* ht, float increase_factor) {
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

    // iterate all the existing entries
    for (int32_t i = 0; i < old_table_size; i++) {
        CompactHashTableEntry_t const * entry = &ht->entries[i];
        // skip empty entries, but also skip deleted entries, b/c those are only needed to re-find
        // the roll-over position of something that was inserted with a collision, but then removed.
        // we'll re-create equivalent conditions by inserting the keys and values again, simply
        // by using the insert fn
        if (entry->key != NULL && !is_deleted_entry_key(entry->key)) {
            compact_hash_table_insert(new_ht, entry->key, entry->key_len, entry->value_type, entry->value);
        }
    }

    compact_hash_table_destroy(ht);

    return new_ht;
}



