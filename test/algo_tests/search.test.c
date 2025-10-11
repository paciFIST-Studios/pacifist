// paciFIST studios. 2025. MIT License

#ifndef SEARCH_TEST_H
#define SEARCH_TEST_H

#include <check.h>

// header
#include "algo/search.h"

// stdlib
#include <stdlib.h>
// framework
// project



// https://stackoverflow.com/a/56966478 
START_TEST(stdlib_bsearch_example__int32) {
    int32 values[] = { 50, 20, 90, 100, 40, 30, 60, 10, 70, 80};
    int32 len = sizeof(values) / sizeof(values[0]);
    int32 target = 10;

    int32* pResult = NULL;
    qsort(values, len, sizeof(int32), compare_int32);
    pResult = (int*) bsearch(&target, values, len, sizeof(int32), compare_int32); 
    ck_assert_ptr_nonnull(pResult);
    ck_assert_int_eq(*pResult, target);
}
END_TEST






#endif //SEARCH_TEST_H
