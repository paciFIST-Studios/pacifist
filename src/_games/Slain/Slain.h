// paciFIST studios. 2025. MIT License

#ifndef SLAIN_H
#define SLAIN_H

// stdlib
// framework
// engine
#include "../../string/pstring.h"
#include <memory/allocator/PFMemoryArena.h>




typedef struct SlainGameLifetimeState_t {
    PString_t window_title;

    PFAllocator_MemoryArena_t * program_lifetime_memory_arena;

    void(*pf_handle_mouse_movement)(float mouse_x, float mouse_y);
    void(*pf_handle_mouse_down)(uint8_t);
    void(*pf_handle_mouse_up)(uint8_t);
    void(*pf_handle_mouse_wheel_scroll)(int16_t scroll);
    void(*pf_handle_key_down)(uint8_t, uint8_t);
    void(*pf_handle_key_up)(uint8_t, uint8_t);



} SlainGameLifetimeState_t;


void slain_game_lifetime_state_initialize(SlainGameLifetimeState_t* slain_game_state);


void pf_handle_mouse_movement(float mouse_x, float mouse_y);
void pf_handle_mouse_down(uint8_t mouse_buttons);
void pf_handle_mouse_up(uint8_t mouse_buttons);
void pf_handle_mouse_wheel_scroll(int16_t scroll);
void pf_handle_key_down(uint8_t key, uint8_t modifiers);
void pf_handle_key_up(uint8_t key, uint8_t modifiers);



#endif //SLAIN_H
