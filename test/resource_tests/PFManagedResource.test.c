// paciFIST Studios. 2025. MIT License.

// includes
#include <check.h>
#include "resource/PFManagedResource.h"

// stdlib
// framework
// engine

// struct PFManagedResource_t --------------------------------------------------------------------------------

START_TEST(fn_struct_PFManagedResource_t__is_defined) {
    PFManagedResource_t const managed_resource = {0};
    ck_assert_ptr_nonnull(&managed_resource);
}
END_TEST

START_TEST(fn_struct_PFManagedResource_t__has_expected_size) {
    ck_assert_int_eq(sizeof(PFManagedResource_t), 16);
}
END_TEST

START_TEST(fn_struct_PFManagedResource_t__has_expected_members) {
    PFManagedResource_t const managed_resource = {0};
    ck_assert_ptr_nonnull(&managed_resource.data_type);
    //ck_assert_ptr_nonnull(&managed_resource.flags);
    ck_assert_ptr_nonnull(&managed_resource.size);
    ck_assert_ptr_nonnull(&managed_resource.data);
}
END_TEST
