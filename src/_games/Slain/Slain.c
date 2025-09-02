// paciFIST studios. 2025. MIT License

// stdlib
// framework
// engine
// game
#include "Slain.h"


void slain_game_lifetime_state_initialize(SlainGameLifetimeState_t* slain_game_state) {
    slain_game_state->pf_handle_mouse_movement = pf_handle_mouse_movement;
    slain_game_state->pf_handle_mouse_down = pf_handle_mouse_down;
    slain_game_state->pf_handle_mouse_up = pf_handle_mouse_up;
    slain_game_state->pf_handle_mouse_wheel_scroll = pf_handle_mouse_wheel_scroll;
    slain_game_state->pf_handle_key_down = pf_handle_key_down;
    slain_game_state->pf_handle_key_up = pf_handle_key_up;
    
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