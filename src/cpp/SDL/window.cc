#include "../../include/SDL/window.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "../../include/SDL/app.hpp"
#include "../../include/SDL/element.hpp"
#include "Debug.h"
#include "Init_list.h"
#include "SDL/SDL_def.hpp"
#include "def.h"

/* Add a element to the window. */
element *
window::new_element(SDL_Color color, SDL_FRect rect, init_list<unsigned> flags) noexcept
{
    element *elem = APP->mem_pool.alloc<element>();
    if (!elem)
    {
        logE("Mem alloc failed, 'new(nothrow)' return`ed 'nullptr'.");
        APP->exit_clean(1);
    }
    elem->color  = color;
    elem->rect   = rect;
    elem->renptr = &ren;
    elem->flags  = flags;
    elem->init();
    elements.push_back(elem);
    logI("mem_pool used: %lu.", APP->mem_pool.used_memory());
    return elem;
}

/* Init win and ren for the window. */
void
window::init(void) noexcept
{
    w = 800;
    h = 600;
    framerate == 0 ? set_framerate(480) : (void)0;
    SDL_CreateWindowAndRenderer(title ? title : "placeholder", w, h, SDL_WINDOW_RESIZABLE, &win, &ren);
    if (!win || !ren)
    {
        logE("Failed to create %s, SDL_ERROR: %s.", win ? "renderer" : "window", SDL_GetError());
        APP->exit_clean(1);
    }
    const char *ren_name = SDL_GetRendererName(ren);
    if (ren_name)
    {
        logI("Renderer name: %s", ren_name);
    }
}

/* Cleanup window. */
void
window::cleanup(void) noexcept
{
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}

/* Set window size. */
void
window::set_size(const float width, const float height) noexcept
{
    SDL_SetWindowSize(win, (int)width, (int)height);
    w = width;
    h = height;
}

/* Set window position. */
void
window::set_position(const int x, const int y) noexcept
{
    SDL_SetWindowPosition(win, x, y);
}

/* Set window title. */
void
window::set_title(const char *str) noexcept
{
    SDL_SetWindowTitle(win, str);
}

/* Set window min size. */
void
window::set_min_size(const int w, const int h) noexcept
{
    SDL_SetWindowMinimumSize(win, w, h);
}

void
window::set_framerate(int newfps) noexcept
{
    this->framerate       = newfps;
    this->sec_frame_delay = FRAMETIME_S(this->framerate);
    this->ms_frame_delay  = FRAME_DELAY(this->framerate);
}
