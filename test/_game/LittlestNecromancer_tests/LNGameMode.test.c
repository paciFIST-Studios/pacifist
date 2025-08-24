// paciFIST studios. 2025. MIT License

// includes
#include <check.h>
#include "../../../src/_games/LittlestNecromancer/LNGameMode.h"

// stdlib
// framework
// engine

// enum LNGameMode_t -----------------------------------------------------------------------------------------

START_TEST(fn_enum_ELNGameMode_t__is_defined) {
    int const result = (int)ELNGM_UNINITIALIZED;
    ck_assert_int_eq(result, 0);
}
END_TEST





