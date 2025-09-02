// paciFIST studios. 2025. MIT License

#ifndef PF_RESOURCE_MANAGER_H
#define PF_RESOURCE_MANAGER_H

// stdlib
// framework
// engine
#include "../memory/allocator/PFAllocator.h"
#include "../memory/hash_table/PFCompactHashTable.h"

#include "../string/pstring.h"

typedef struct PFResourceManager_t {
    //PFAllocator_t allocator;
    //PFCompactHashTable_t resource_hash_table;

    // registers the resource with the resource manager.  The file name of this asset is used 
    // to identify it in the hash map.  Can choose to immediately load the resource or not.
    // resources are loaded into arrays of similarly typed objects
    int32_t (*register_resource_with_path)(PString_t const path, int32_t const bAutoLoadResource);

    // if the resource is not loaded, loads the resource into the given array
    int32_t (*load_resource_with_name)(PString_t const resource_name);

    // returns the resource discovered with this name, if any
    void* (*get_resource_with_name)(PString_t const resource_name);

    // unloads the resource discovered with this name, if any, but does not unregister it
    // if memory compaction occurs, this resource will not lose its place
    int32_t (*unload_resource_with_name)(PString_t const resource_name);

    // unloads the resource discovered with this name, if any, and if memory compaction occurs,
    // this resource *will* lose its place
    int32_t (*unregister_resource_with_name)(PString_t const resource_name);

} PFResourceManager_t;


int32_t pf_resource_manager_initialize(void* memory, PFResourceManager_t* resource_manager);

int32_t pf_resource_manager_register_resource_with_path(PString_t const path, int32_t const bAutoLoadResource);
int32_t pf_resource_manager_load_resource_with_name(PString_t const resource_name);
void* pf_resource_manager_get_resource_with_name(PString_t const resource_name);
int32_t pf_resource_manager_unload_resource_with_name(PString_t const resource_name);
int32_t pf_resource_manager_unregister_resource_with_name(PString_t const resource_name);




#endif // PF_RESOURCE_MANAGER_H
