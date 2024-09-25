#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <sys/cdefs.h>

#include "../Attributes.h"
#include "../Flag.h"
#include "../Init_list.h"
#include "../def.h"

#include "v2d.hpp"

struct element_text
{
    float x      = -1;
    float y      = -1;
    float w      = 0;
    float h      = 0;
    Uint  format = 0;
    int   access = 0;
    SDL_Texture *texture          = nullptr;
    SDL_Texture *highight_texture = nullptr;
    /*-<< Methods >>-*/
    void query_text_texture(float x = -1, float y = -1, SDL_Texture *t = nullptr) noexcept;
};

struct element
{
    enum state_flags
    {
        IN_ANIMATION = 1,
        ALIGN_X,
        ALIGN_Y,
        ALIGN_WIDTH,
        ALIGN_HEIGHT,
        SHOULD_FEEL_GRAVITY,
        MOUSE_INSIDE,
        HIGHLIGHT_ON_HOVER,
        IS_STATIC, // IS_GROUND
        FLAG_SIZE = 16
    };
    SDL_FRect             rect;
    vector2f              vel;
    SDL_FRect             align_rect;
    bit_flag_t<FLAG_SIZE> flags;
    SDL_Color             color;
    SDL_Renderer        **renptr;
    element_text         *text_data;
    /*-<< Methods >>-*/
    void init(void) noexcept;
    void draw(void) noexcept;
    void add_text_data(float x, float y, SDL_Texture *texture) noexcept;
    void add_text_data(TTF_Font *font, float x, float y, SDL_Color fg, SDL_Color bg,
                       const char *str) noexcept;
    void animate(float end_x, float end_y, float end_w, float end_h, int duration_ms) noexcept;
    void set_borders(float size, SDL_Color color) noexcept;
    void set_highlight_on_hover(SDL_Color color, SDL_Color border_color) noexcept;
    void set_text_highlight_on_hover(SDL_Color fg, SDL_Color bg) noexcept;

    std::function<void(SDL_MouseButtonEvent ev)> _action = nullptr;

    template <typename F>
    __inline__ void
    __attr(__always_inline__, __nodebug__, __nothrow__) mouse_click_action(F &&f) noexcept
    {
        _action = std::forward<F>(f);
        _add_callback();
    }

    /*-<< Constructor >>-*/
    element(SDL_Color color, SDL_FRect rect) noexcept;
    element(void) noexcept;

private:
    float     border_size;
    SDL_Color border_color;
    SDL_Color highlight_color;
    SDL_Color highlight_border_color;
    /*-<< Methods >>-*/
    void draw_borders(void) noexcept;
    void _gravity(void) noexcept;
    void _add_callback(void) noexcept;
};
