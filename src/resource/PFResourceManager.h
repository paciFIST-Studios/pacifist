// paciFIST studios. 2025. MIT License

#ifndef PF_RESOURCE_MANAGER_H
#define PF_RESOURCE_MANAGER_H

// stdlib
// framework
// engine
#include "memory/allocator/PFMemoryArena.h"
#include "memory/allocator/PFAllocator.h"
#include "memory/hash_table/PFCompactHashTable.h"
#include "resource/PFManagedResource.h"
#include "string/pstring.h"



#define RESOURCE_MANAGER_RESOURCE_COUNT 32


typedef struct PFResourceManagerConfiguration_t {
    size_t total_managed_resource_count;
    size_t lifetime_allocator_size;
    size_t dynamic_allocator_size;
} PFResourceManagerConfiguration_t;



typedef struct PFResourceManager_t {

    // things which won't ever be un-allocated should go here.  For example, the memory for the
    // PFManagedResource_t array
    PFAllocator_MemoryArena_t* lifetime_allocator;

    PFAllocator_FreeList_t* dynamic_allocator;

    // a linear array of all resources managed by this resource manager
    PFManagedResource_t* managed_resource_array;

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


/**
 * @brief Creates a resource manager using the supplied memory, according to the supplied configuration
 *
 * @param base_memory 
 * @param memory_size 
 * @param config 
 * @return 
 */
PFResourceManager_t* pf_resource_manager_create_with_memory(
    void* base_memory,
    size_t memory_size,
    PFResourceManagerConfiguration_t config);


int32_t pf_resource_manager_register_resource_with_path(PString_t path, int32_t bAutoLoadResource);


int32_t pf_resource_manager_load_resource_with_name(PString_t resource_name);


void* pf_resource_manager_get_resource_with_name(PString_t resource_name);


int32_t pf_resource_manager_unload_resource_with_name(PString_t resource_name);


int32_t pf_resource_manager_unregister_resource_with_name(PString_t resource_name);




#endif // PF_RESOURCE_MANAGER_H
