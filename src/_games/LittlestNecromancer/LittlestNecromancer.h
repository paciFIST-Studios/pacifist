// paciFIST studios. 2025. MIT License

#ifndef LITTLEST_NECROMANCER_H
#define LITTLEST_NECROMANCER_H

// stdlib
// framework
// engine
#include "../../string/pstring.h"
#include "../../memory/allocator/MemoryArena.h"




typedef struct LittlestNecromancerGameLifetimeState_t {
    PString_t window_title;

    MemoryArena_t * program_lifetime_memory_arena;

    void(*pf_handle_mouse_movement)(float mouse_x, float mouse_y);
    void(*pf_handle_mouse_down)(uint8_t);
    void(*pf_handle_mouse_up)(uint8_t);
    void(*pf_handle_mouse_wheel_scroll)(int16_t scroll);
    void(*pf_handle_key_down)(uint8_t, uint8_t);
    void(*pf_handle_key_up)(uint8_t, uint8_t);

    
    
} LittlestNecromancerGameLifetimeState_t;


void littlest_necromancer_game_lifetime_state_initialize(LittlestNecromancerGameLifetimeState_t* ln_game_state);


void pf_handle_mouse_movement(float mouse_x, float mouse_y);
void pf_handle_mouse_down(uint8_t mouse_buttons);
void pf_handle_mouse_up(uint8_t mouse_buttons);
void pf_handle_mouse_wheel_scroll(int16_t scroll);
void pf_handle_key_down(uint8_t key, uint8_t modifiers);
void pf_handle_key_up(uint8_t key, uint8_t modifiers);



#endif //LITTLEST_NECROMANCER_H
