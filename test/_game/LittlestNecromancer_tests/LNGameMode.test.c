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

START_TEST(fn_enum_ELNGameMode_t__has_expected_number_of_states) {
    int const result = (int)ELNGM_COUNT;
    ck_assert_int_eq(result, 17);
}
END_TEST

// ELNGM_APPLICATION_STARTUP
START_TEST(fn_enum_ELNGameMode_t__application_startup_value__is_defined) {
    int const result = (int)ELNGM_APPLICATION_STARTUP;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_APPLICATION_MAIN_MENU
START_TEST(fn_enum_ELNGameMode_t__application_main_menu_value__is_defined) {
    int const result = (int)ELNGM_APPLICATION_MAIN_MENU;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_APPLICATION_DEMO
START_TEST(fn_enum_ELNGameMode_t__application_demo_value__is_defined) {
    int const result = (int)ELNGM_APPLICATION_DEMO;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_APPLICATION_LOADING
START_TEST(fn_enum_ELNGameMode_t__application_loading_value__is_defined) {
    int const result = (int)ELNGM_APPLICATION_LOADING;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_APPLICATION_SETTINGS
START_TEST(fn_enum_ELNGameMode_t__application_settings_value__is_defined) {
    int const result = (int)ELNGM_APPLICATION_SETTINGS;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_APPLICATION_ABOUT
START_TEST(fn_enum_ELNGameMode_t__application_about_value__is_defined) {
    int const result = (int)ELNGM_APPLICATION_ABOUT;
    ck_assert_int_ne(result, 0);
}
END_TEST


// ELNGM_GAMEPLAY
START_TEST(fn_enum_ELNGameMode_t__gameplay_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_GAMEPLAY_PAUSE
START_TEST(fn_enum_ELNGameMode_t__gameplay_pause_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY_PAUSE;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_GAMEPLAY_CHARACTER_CREATOR
START_TEST(fn_enum_ELNGameMode_t__gameplay_character_creator_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY_CHARACTER_CREATOR;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_GAMEPLAY_DECORATE_AREA
START_TEST(fn_enum_ELNGameMode_t__gameplay_decorate_area_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY_DECORATE_AREA;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_GAMEPLAY_DIALOGUE
START_TEST(fn_enum_ELNGameMode_t__gameplay_dialogue_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY_DIALOGUE;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_GAMEPLAY_INVENTORY
START_TEST(fn_enum_ELNGameMode_t__gameplay_inventory_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY_INVENTORY;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_GAMEPLAY_NPC_FRIENDS
START_TEST(fn_enum_ELNGameMode_t__gameplay_npc_friends_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY_NPC_FRIENDS;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_GAMEPLAY_QUEST_TRACKER
START_TEST(fn_enum_ELNGameMode_t__gameplay_quest_tracker_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY_QUEST_TRACKER;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_GAMEPLAY_TRADE
START_TEST(fn_enum_ELNGameMode_t__gameplay_trade_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY_TRADE;
    ck_assert_int_ne(result, 0);
}
END_TEST

// ELNGM_GAMEPLAY_WORLD_MAP_NAVIGATION
START_TEST(fn_enum_ELNGameMode_t__gameplay_world_map_navigation_value__is_defined) {
    int const result = (int)ELNGM_GAMEPLAY_WORLD_MAP_NAVIGATION;
    ck_assert_int_ne(result, 0);
}
END_TEST

