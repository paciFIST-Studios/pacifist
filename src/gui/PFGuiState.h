// paciFIST studios. 2025. MIT License

#ifndef PF_GUI_STATE_H
#define PF_GUI_STATE_H

// stdlib
#include <stdint.h>
// framework
// engine


// class should contain everything needed to draw the mouse,
// as well as the state of objects interacting with the mouse
typedef struct PFGuiState_t {
    float mouse_x;
    float mouse_y;
    int16_t mouse_wheel_scroll;
    uint8_t mouse1_is_down : 0x01;
    uint8_t mouse2_is_down : 0x02;
    uint8_t mouse3_is_down : 0x03;

    // the item below the mouse
    int32_t hovered_item_id;
    // single item currently interacting with the mouse
    // if an item is selected, and you move the mouse away, it remains
    // selected, but it stops being hovered
    int32_t selected_item_id;

} PFGuiState_t;




#endif //PF_GUI_STATE_H