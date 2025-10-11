// paciFIST studios. 2025. MIT License

// include
#include "PFResourceManager.h"

// stdlib
// framework
// engine
#include "core/common.h"
#include "core/error.h"


/**
 *  Resource Manager Todo:
 *      A config file which we can load in for the project
 *          project config specifies asset config
 *      A config file we can load in for the assets
 *          shows:
 *              path
 *              level-metadata
 *              is-register-immediate
 *              is-load-immediate
 *              try-load-asset-as
 *              asset-budget
 *              engine-labels
 *
 *      Resource Manager runs at engine start
 *      Resource Manager pulls in assets from config at start
 *      Resource Manager loads assets which are marked as load-immediate
 *      Resource Manager logs what it does
 * 
 */







/**
 * 
 * @param base_memory 
 * @param memory_size 
 * @param config 
 * @return 
 */
PFResourceManager_t* pf_resource_manager_create_with_memory(
    void* base_memory,
    size_t const memory_size,
    PFResourceManagerConfiguration_t const config)
{
    if (base_memory == NULL) {
        PF_LOG_CRITICAL(PF_RESOURCE, "PFResourceManager_t got ptr to NULL base memory!");
        return NULL;
    }

    size_t const required_memory = (config.total_managed_resource_count * sizeof(PFManagedResource_t))
                                    + sizeof(PFResourceManager_t)
                                    + sizeof(PFAllocator_FreeList_t)
                                    ;
    if (memory_size <= required_memory) {
        char error_message[128];
        sprintf(error_message, "Not provided enough memory to initialize PFResourceManager_t! Requires: %lu, Got: %lu", required_memory, memory_size);
        PF_LOG_CRITICAL(PF_RESOURCE, error_message);
        return NULL;
    }

    // set the resource manager at the base of the memory it owns
    PFResourceManager_t* resource_manager = (PFResourceManager_t*)base_memory;

    // 
    size_t const base_memory_start = (size_t)base_memory;

    // usable memory, starting after the resource manager
    size_t const post_rm_offset = base_memory_start + sizeof(PFResourceManager_t);

    resource_manager->lifetime_allocator = pf_allocator_memory_arena_create_with_memory(
        (void *)post_rm_offset, config.lifetime_allocator_size);

    if (resource_manager->lifetime_allocator == NULL) {
        PF_LOG_CRITICAL(PF_RESOURCE, "Could not create PFAllocator_MemoryArena_t as lifetime allocator! Cannot create PFResourceManager_t!");
        return NULL;
    }

    // usable memory, starting after the lifetime allocator
    size_t const post_la_offset = post_rm_offset + sizeof(PFAllocator_MemoryArena_t) + config.lifetime_allocator_size;

    resource_manager->dynamic_allocator = pf_allocator_free_list_create_with_memory(
        (void*)post_la_offset, post_la_offset - base_memory_start);

    if (resource_manager->dynamic_allocator == NULL) {
        PF_LOG_CRITICAL(PF_RESOURCE, "Failed to initialize allocator! Cannot complete initialization of PFResourceManager!");
        return NULL;
    }

    //https://floooh.github.io/2018/06/17/handles-vs-pointers.html 


    

    return resource_manager;
}

int32_t pf_resource_manager_register_resource_with_path(PString_t const path,
    int32_t const bAutoLoadResource) {
    return TRUE;
}

int32_t pf_resource_manager_load_resource_with_name(PString_t const resource_name) {
    return TRUE;
}

void * pf_resource_manager_get_resource_with_name(PString_t const resource_name) {
    return NULL;
}

int32_t pf_resource_manager_unload_resource_with_name(PString_t const resource_name) {
    return TRUE;
}

int32_t pf_resource_manager_unregister_resource_with_name(PString_t const resource_name) {
    return TRUE;
}
