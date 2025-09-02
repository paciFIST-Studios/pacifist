// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "../../src/resource/PFResourceManager.h"

// stdlib
// framework
// engine

// struct PFResourceManager_t --------------------------------------------------------------------------------

START_TEST(fn_struct_PFResourceManager_t__is_defined) {
    PFResourceManager_t const resource_manager = {0};
    ck_assert_ptr_nonnull(&resource_manager);
}
END_TEST

START_TEST(fn_struct_PFResourceManager_t__has_expected_size) {
    ck_assert_int_eq(sizeof(PFResourceManager_t), 40);
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
    // exists
    // sets up members correctly
    // sets up allocator using provided data
    // correct errors for invalid args

// fn pf_resource_manager_register_resource_with_path --------------------------------------------------------
    // doesn't work for non-init
    // works for params
    // correct errors for invalid args
        // not a real path
        // file at path is unrecognized type (load as bytes?)
        // invalid args


// fn pf_resource_manager_get_resource_with_name -------------------------------------------------------------
    // doesn't work for non-init
    // returns intelligible value if resource isn't there
    // returns resource if it is there
    // returns valid resource if holding a valid resource
    // returned resource has correct type
    // correct errors for invalid args
    // correct errors of issues during operation


// fn pf_resource_manager_unload_resource_with_name ----------------------------------------------------------
    // doesn't work for non-init
    // doesn't work if that resource is not loaded
    // doesn't work if that resource is not registered
    // if resource is loaded, then it's unloaded and its memory is zeroed out right then


// fn pf_resource_manager_unregister_resource_with_name ------------------------------------------------------
    // doesn't work for non-init
    // doesn't work if the resource is not registered
    // throws error if resource is loaded, and you call unregister
    // if resource is registered, but not loaded, then it unregisters

