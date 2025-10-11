// paciFIST studios. 2025. MIT License

#ifndef PACIFIST_RENDER_H
#define PACIFIST_RENDER_H

// stdlib
// framework
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
// engine
// game


__attribute__((unused))
static void
pf_draw_rect(
    SDL_Surface* sdl_surface,
    int32_t const x,
    int32_t const y,
    int32_t const w,
    int32_t const h,
    int32_t const color)
{
    SDL_Rect const rect = {
        .x = x,
        .y = y,
        .w = w,
        .h = h
    };
    SDL_FillSurfaceRect(sdl_surface, &rect, color);
}

__attribute__((unused))
static void pf_draw_circle(void) {
    // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
    // https://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture6/circle.html 
}

__attribute__((unused))
static void pf_draw_ellipse(void) {
    // https://en.wikipedia.org/wiki/Ellipse#Applications
}

__attribute__((unused))
static void pf_draw_curve(void) {
    // https://en.wikipedia.org/wiki/Curve
}

__attribute__((unused))
static void pf_draw_line_hard(void) {
    //https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm 
}

__attribute__((unused))
static void pf_draw_line_aa(void) {
    // https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm 
}

__attribute__((unused))
static void pf_draw_spline_cubic(void) {}

__attribute__((unused))
static void pf_draw_spline_quadratic(void) {
    // https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-25-rendering-vector-art-gpu
    // https://www.jakelow.com/blog/hobby-curves 
}

#endif //PACIFIST_RENDER_H