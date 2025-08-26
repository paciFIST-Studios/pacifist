// paciFIST studios. 2025. MIT License

// stdlib
// framework
// engine
// game
#include "LittlestNecromancer.h"


void littlest_necromancer_game_lifetime_state_initialize(LittlestNecromancerGameLifetimeState_t* ln_game_state) {
    ln_game_state->pf_handle_mouse_movement = pf_handle_mouse_movement;
    ln_game_state->pf_handle_mouse_down = pf_handle_mouse_down;
    ln_game_state->pf_handle_mouse_up = pf_handle_mouse_up;
    ln_game_state->pf_handle_mouse_wheel_scroll = pf_handle_mouse_wheel_scroll;
    ln_game_state->pf_handle_key_down = pf_handle_key_down;
    ln_game_state->pf_handle_key_up = pf_handle_key_up;
    
}


void pf_handle_mouse_movement(float mouse_x, float mouse_y) {
    
}

void pf_handle_mouse_down(uint8_t mouse_buttons) {
    
}

void pf_handle_mouse_up(uint8_t mouse_buttons) {
    
}

void pf_handle_mouse_wheel_scroll(int16_t scroll) {
    
}

void pf_handle_key_down(uint8_t key, uint8_t modifiers) {
    
}

void pf_handle_key_up(uint8_t key, uint8_t modifiers) {
    
}