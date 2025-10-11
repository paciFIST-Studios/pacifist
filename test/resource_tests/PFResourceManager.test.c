// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "resource/PFResourceManager.h"

// stdlib
// framework
// engine
#include "core/define.h"

// struct PFResourceManagerConfiguration_t -------------------------------------------------------------------

START_TEST(fn_struct_PFResourceManagerConfiguration_t__is_defined) {
    PFResourceManagerConfiguration_t const config = {0};
    ck_assert_ptr_nonnull(&config);
}
END_TEST

START_TEST(fn_struct_PFResourceManagerConfiguration_t__has_expected_size) {
    ck_assert_int_eq(sizeof(PFResourceManagerConfiguration_t), 24);
}
END_TEST

START_TEST(fn_struct_PFResourceManagerConfiguration_t__has_expected_members) {
    PFResourceManagerConfiguration_t const config = {0};
    ck_assert_ptr_nonnull(&config.total_managed_resource_count);
}
END_TEST

// struct PFResourceManager_t --------------------------------------------------------------------------------

START_TEST(fn_struct_PFResourceManager_t__is_defined) {
    PFResourceManager_t const resource_manager = {0};
    ck_assert_ptr_nonnull(&resource_manager);
}
END_TEST

START_TEST(fn_struct_PFResourceManager_t__has_expected_size) {
    ck_assert_int_eq(sizeof(PFResourceManager_t), 64);
}
END_TEST

START_TEST(fn_struct_PFResourceManager_t__has_expected_members) {
    PFResourceManager_t const resource_manager = {0};
    ck_assert_ptr_nonnull(&resource_manager);
    ck_assert_ptr_null(resource_manager.register_resource_with_path);
    ck_assert_ptr_null(resource_manager.load_resource_with_name);
    ck_assert_ptr_null(resource_manager.get_resource_with_name);
    ck_assert_ptr_null(resource_manager.unload_resource_with_name);
    ck_assert_ptr_null(resource_manager.unregister_resource_with_name);
}
END_TEST

// fn pf_resource_manager_initialize -------------------------------------------------------------------------

START_TEST(fn_pf_resource_manager_create_with_memory__is_defined) {
    PFResourceManager_t* (*fptr)(void*, size_t const, PFResourceManagerConfiguration_t const) = pf_resource_manager_create_with_memory;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_pf_resource_manager_create_with_memory__returns_null__for_null_base_memory_ptr) {
    PFResourceManagerConfiguration_t const cfg = {0};
    ck_assert_ptr_null(pf_resource_manager_create_with_memory(NULL, 0,  cfg));
}
END_TEST


START_TEST(fn_pf_resource_manager_create_with_memory__also_creates_resource_manager_allocator__when_called) {
    size_t const memory_size = 4096;
    void* test_memory = malloc(memory_size);
    memset(test_memory, 0, memory_size);

    PFResourceManagerConfiguration_t const cfg = { .total_managed_resource_count = 32 };
    PFResourceManager_t* resource_manager = pf_resource_manager_create_with_memory(test_memory, memory_size, cfg);
    ck_assert_ptr_nonnull(resource_manager);
    
    // free_list_allocator
//    ck_assert_ptr_nonnull(resource_manager->dynamic_allocator->base_memory);
//    ck_assert_int_eq(memory_size-sizeof(PFResourceManager_t), resource_manager->dynamic_allocator->base_memory_size);
//    ck_assert_int_eq(0, resource_manager->dynamic_allocator->free_memory);
//    ck_assert_ptr_nonnull(resource_manager->dynamic_allocator->head);
//    ck_assert_ptr_nonnull(resource_manager->dynamic_allocator->pf_malloc);
//    ck_assert_ptr_nonnull(resource_manager->dynamic_allocator->pf_realloc);
//    ck_assert_ptr_nonnull(resource_manager->dynamic_allocator->pf_free);

    free(test_memory);
}
END_TEST

    // sets up members correctly
    // sets up allocator using provided data
    // correct errors for invalid args

// fn pf_resource_manager_register_resource_with_path --------------------------------------------------------

START_TEST(fn_pf_resource_manager_register_resource_with_path__is_defined) {
    int32_t (*fptr)(PString_t const, int32_t const) = pf_resource_manager_register_resource_with_path;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

    // doesn't work for non-init
    // works for params
    // correct errors for invalid args
        // not a real path
        // file at path is unrecognized type (load as bytes?)
        // invalid args

// fn pf_resource_manager_load_resource_with_name ------------------------------------------------------------

START_TEST(fn_pf_resource_manager_load_resource_with_name__is_defined) {
    int32_t (*fptr)(PString_t const) = pf_resource_manager_load_resource_with_name;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// fn pf_resource_manager_get_resource_with_name -------------------------------------------------------------

START_TEST(fn_pf_resource_manager_get_resource_with_name__is_defined) {
    void* (*fptr)(PString_t const) = pf_resource_manager_get_resource_with_name;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


    // doesn't work for non-init
    // returns intelligible value if resource isn't there
    // returns resource if it is there
    // returns valid resource if holding a valid resource
    // returned resource has correct type
    // correct errors for invalid args
    // correct errors of issues during operation


// fn pf_resource_manager_unload_resource_with_name ----------------------------------------------------------

START_TEST(fp_pf_resource_manager_unload_resource_with_name__is_defined) {
    int32_t (*fptr)(PString_t const) = pf_resource_manager_unload_resource_with_name;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

    // doesn't work for non-init
    // doesn't work if that resource is not loaded
    // doesn't work if that resource is not registered
    // if resource is loaded, then it's unloaded and its memory is zeroed out right then


// fn pf_resource_manager_unregister_resource_with_name ------------------------------------------------------

START_TEST(fn_pf_resource_manager_unregister_resource_with_name__is_defined) {
    int32_t(*fptr)(PString_t const) = pf_resource_manager_unregister_resource_with_name;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

    // doesn't work for non-init
    // doesn't work if the resource is not registered
    // throws error if resource is loaded, and you call unregister
    // if resource is registered, but not loaded, then it unregisters

