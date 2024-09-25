#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "Debug.h"
#include "Init_list.h"
#include "SDL/v2d.hpp"
#include "def.h"

#include "SDL/SDL_def.hpp"
#include "SDL/app.hpp"
#include "SDL/element.hpp"
#include "SDL/event_handler.hpp"
#include "SDL/utils.hpp"

enum element_internal_state
{
    MOUSE_INSIDE = 1,
    HIGHLIGHT_ON_HOVER
};

/*-<< sdl_element_text >>-*/
void
element_text::query_text_texture(float x, float y, SDL_Texture *t) noexcept
{
    (x != -1) ? this->x = x : 0;
    (y != -1) ? this->y = y : 0;
    (t != nullptr) ? texture = t : nullptr;
    if (!SDL_GetTextureSize(texture, &w, &h))
    {
        logE("Failed to query texture, SDL_ERROR: %s.", SDL_GetError());
    }
}

/*-<< sdl_element >>-*/
void
element::init(void) noexcept
{}

void
element::draw_borders(void) noexcept
{
    if (border_size <= 0 || border_size >= rect.w || border_size >= rect.h)
    {
        return;
    }
    flags.is_set<HIGHLIGHT_ON_HOVER>() && flags.is_set<MOUSE_INSIDE>()
        ? set_ren_color(*renptr, &highlight_border_color)
        : set_ren_color(*renptr, &border_color);
    SDL_FRect r;
    /* Left border. */
    r = {rect.x, rect.y, border_size, rect.h};
    SDL_RenderFillRect(*renptr, &r);
    /* Right border. */
    r = {rect.x + rect.w - border_size, rect.y, border_size, rect.h};
    SDL_RenderFillRect(*renptr, &r);
    /* Top border. */
    r = {rect.x, rect.y, rect.w, border_size};
    SDL_RenderFillRect(*renptr, &r);
    /* Bottom border. */
    r = {rect.x, rect.y + rect.h - border_size, rect.w, border_size};
    SDL_RenderFillRect(*renptr, &r);
}

void
element::draw(void) noexcept
{
    (flags.is_set<HIGHLIGHT_ON_HOVER>() && flags.is_set<MOUSE_INSIDE>())
        ? set_ren_color(*renptr, &highlight_color)
        : set_ren_color(*renptr, &color);
    SDL_RenderFillRect(*renptr, &rect);
    flags.is_set<SHOULD_FEEL_GRAVITY>() ? _gravity() : (void)0;
    if (text_data)
    {
        if (!text_data->texture)
        {
            logE("texture is nullptr.");
        }
        else
        {
            if (text_data->texture != texture_exceptions::blank_texture)
            {
                SDL_FRect r = {
                    rect.x + text_data->x, rect.y + text_data->y, text_data->w, text_data->h};
                if (!SDL_RenderTexture(*renptr, text_data->texture, nullptr, &r))
                {
                    logE("SDL_RenderTexture. SDL_ERROR: %s.", SDL_GetError());
                }
            }
        }
    }
    draw_borders();
}

void
element::add_text_data(float x, float y, SDL_Texture *texture) noexcept
{
    if (texture == nullptr)
    {
        logE("texture == nullptr.");
        return;
    }
    if (text_data == nullptr)
    {
        text_data = APP->mem_pool.alloc<element_text>();
        if (text_data == nullptr)
        {
            APP->exit_clean(1);
        }
    }
    text_data->query_text_texture(x, y, texture);
}

void
element::add_text_data(TTF_Font *font, float x, float y, SDL_Color fg, SDL_Color bg,
                       const char *str) noexcept
{
    if (str == nullptr)
    {
        logE("str == nullptr.");
        return;
    }
    if (text_data == nullptr)
    {
        text_data = APP->mem_pool.alloc<element_text>();
        if (text_data == nullptr)
        {
            APP->exit_clean(1);
        }
    }
    text_data->texture = APP->make_destroy_text_texture(text_data->texture, font, str, fg, bg);
    text_data->query_text_texture(x, y, text_data->texture);
}

void
element::animate(float end_x, float end_y, float end_w, float end_h, int duration_ms) noexcept
{
    if (!flags.is_set<IN_ANIMATION>())
    {
        float steps  = (duration_ms / APP->root->ms_frame_delay);
        float step_x = (end_x - rect.x) / steps;
        float step_y = (end_y - rect.y) / steps;
        float step_w = (end_w - rect.w) / steps;
        float step_h = (end_h - rect.h) / steps;
        logI("steps: %f", steps);
        logI("step_x: %f", step_x);
        logI("step_y: %f", step_y);
        logI("step_w: %f", step_w);
        logI("step_h: %f", step_h);
        element_animation_data animation_data {
            steps, end_x, end_y, end_w, end_h, step_x, step_y, step_w, step_h,
        };
        APP->animate_element(this, animation_data);
    }
}

void
element::set_borders(float size, SDL_Color color) noexcept
{
    border_size  = size;
    border_color = color;
}

void
element::set_highlight_on_hover(SDL_Color color, SDL_Color border_color) noexcept
{
    highlight_color        = color;
    highlight_border_color = border_color;
    flags.set<HIGHLIGHT_ON_HOVER>();
    EVENT_HANDLER->event_action(SDL_EVENT_MOUSE_MOTION, [this](SDL_Event ev) {
        SDL_MouseMotionEvent e = ev.motion;
        if (((e.x >= rect.x && e.x <= rect.x + rect.w) &&
             (e.y >= rect.y && e.y <= rect.y + rect.h)))
        {
            flags.set<MOUSE_INSIDE>();
        }
        else /*  if (state.is_set<MOUSE_INSIDE>()) */
        {
            flags.unset<MOUSE_INSIDE>();
        }
    });
}

void 
element::set_text_highlight_on_hover(SDL_Color fg, SDL_Color bg) noexcept
{
    if (text_data == nullptr || text_data->texture == nullptr)
    {
        logE("text_data == nullptr || text_data->texture == nullptr");
        return;
    }
}

/* Constructors. */
element::element(SDL_Color c, SDL_FRect r) noexcept
{
    color       = c;
    rect        = r;
    text_data   = nullptr;
    renptr      = nullptr;
    border_size = 0;
}

element::element(void) noexcept
    : rect {0, 0, 0, 0}, align_rect {0, 0, 0, 0}, color {0, 0, 0, 0}, renptr(nullptr),
      text_data(nullptr), border_size(0), border_color {0, 0, 0, 0}, highlight_color {0, 0, 0, 0}
{}

/*-<< sdl_button_element >>-*/
void
element::_add_callback(void) noexcept
{
    EVENT_HANDLER->event_action(SDL_EVENT_MOUSE_BUTTON_DOWN, [this](SDL_Event ev) {
        SDL_MouseButtonEvent e = ev.button;
        if (((e.x >= rect.x && e.x <= (rect.x + rect.w)) &&
             (e.y >= rect.y && e.y <= (rect.y + rect.h))))
        {
            if (_action)
            {
                _action(e);
            }
        }
    });
}

void
element::_gravity(void) noexcept
{
    ACCEL(vel.y, GRAVITY, APP->root->sec_frame_delay);
    /* Now calculate new position based on velocity. */
    POS(rect.x, M_TO_PIXEL(vel.x), APP->root->sec_frame_delay);
    POS(rect.y, M_TO_PIXEL(vel.y), APP->root->sec_frame_delay);
    for (const auto &it : APP->root->elements)
    {
        if (it == this || !it->flags.is_set<IS_STATIC>())
        {
            continue;
        }
        if (ELEMS_COLLIDING(this, it))
        {
            if (OBJ_OVERLAP_LEAST_TOP(this, it))
            {
                (this)->rect.y = ELEM_TOP(it) - (this)->rect.h;
                CONSTRAIN_VEL_IF(((this)->vel.y > 0.00), (this)->vel.y)
                // ((*object)->state |= ON_STATIC_OBJECT);
                // /* Apply friction if 'static_object' has any. */
                // if ((*static_object)->accel.y > 0.00)
                // {
                //     if ((*object)->vel.x > 0.00)
                //     {
                //         ACCEL((*object)->vel.x, -(*static_object)->accel.y, TIME_STEP_S);
                //         CONSTRAIN_VEL_IF(((*object)->vel.x < 0.00), (*object)->vel.x);
                //     }
                //     else if ((*object)->vel.x < 0.00)
                //     {
                //         ACCEL((*object)->vel.x, (*static_object)->accel.y, TIME_STEP_S);
                //         CONSTRAIN_VEL_IF(((*object)->vel.x > 0.00), (*object)->vel.x)
                //     }
                // }
                return;
            }
            else if (OBJ_OVERLAP_LEAST_BOT(this, it))
            {
                ELEM_TOP(this) = ELEM_BOT(it);
                CONSTRAIN_VEL_IF(((this)->vel.y < 0.00), (this)->vel.y);
            }
            else if (OBJ_OVERLAP_LEAST_LEFT(this, it))
            {
                (this)->rect.x = ELEM_LEFT(it) - (this)->rect.w;
                CONSTRAIN_VEL_IF(((this)->vel.x > 0.00), (this)->vel.x);
            }
            else if (OBJ_OVERLAP_LEAST_RIGHT(this, it))
            {
                ELEM_LEFT(this) = ELEM_RIGHT(it);
                CONSTRAIN_VEL_IF(((this)->vel.x < 0.00), (this)->vel.x);
            }
        }
    }
}
