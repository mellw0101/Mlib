#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "../Attributes.h"
#include "../Init_list.h"
#include "../Vector.h"
#include "../def.h"

#include "element.hpp"

/* clang-format off */
struct window {
    /* Data vars. */
    SDL_Window   *win = nullptr;
    SDL_Renderer *ren = nullptr;
    const char *title = nullptr;
    float sec_frame_delay;
    float ms_frame_delay;
    float w = 800;
    float h = 600;

    MVector<element *> elements;
    
    /* Methods. */
    element *__warn_unused
    new_element(SDL_Color color, SDL_FRect rect, init_list<Uint> flags) noexcept;
    
    void init(void)                          noexcept;
    void cleanup(void)                       noexcept;
    void set_size(float width, float height) noexcept;
    void set_position(int x, int y)          noexcept;
    void set_title(const char *str)          noexcept;
    void set_min_size(int w, int h)          noexcept;
    void set_framerate(int newfps)           noexcept;

private:
    int framerate = 0;
};
/* clang-format on */
