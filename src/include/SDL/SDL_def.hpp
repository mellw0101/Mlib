#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <chrono>
#include <functional>
#include <ratio>

#include "../Flag.h"
#include "../Mem_pool.h"
#include "../Vector.h"

#include "element.hpp"
#include "key_handler.hpp"
#include "window.hpp"

using std::milli;
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::time_point;
#define __this_thread std::this_thread
template <typename T>
using func = std::function<T>;

#define ACCEL(velocity, accelaration, delta_t) velocity = velocity + (accelaration * delta_t)
#define POS(position, velocity, delta_t)       position = position + (velocity * delta_t);
#define CLAMP(value, min, max)                 (value = (value < min) ? min : (value > max) ? max : value)

#define PIXELS_PER_METER                       100.00
#define PIXEL_TO_M(pixel)                      (pixel / PIXELS_PER_METER)
#define M_TO_PIXEL(m)                          (m * PIXELS_PER_METER)
#define GRAVITY                                9.81
#define FRAMETIME_S(fps)                       (1.00 / fps)

#define NOPOS                                  {0, 0, 0, 0}
#define NOCOLOR                                {0, 0, 0, 0}
#define WHITE                                  {255, 255, 255, 255}
#define BLACK                                  {0, 0, 0, 255}
#define RED                                    {255, 0, 0, 255}
#define BLUE                                   {0, 0, 255, 255}
#define GREEN                                  {0, 255, 0, 255}

/* If condition is 'TRUE' then set value to 0.00. */
#define CONSTRAIN_VEL_IF(condition, value)     (condition) ? (value = 0.00) : 0;
/* This macro is used to confine an object within a reqtangle. */
#define HIT_WALL(x, velx, min_x, max_x, y, vely, min_y, max_y) \
    CLAMP(x, min_x, max_x);                                    \
    (x == min_x) ? (velx < 0.00) ? (velx = 0.00) : 0 : 0;      \
    (x == max_x) ? (velx > 0.00) ? (velx = 0.00) : 0 : 0;      \
    CLAMP(y, min_y, max_y);                                    \
    (y == min_y) ? (vely < 0.00) ? (vely = 0.00) : 0 : 0;      \
    (y == max_y) ? (vely > 0.00) ? (vely = 0.00) : 0 : 0

#define FRAME_DELAY(framerate) ((float)1000 / framerate)

#define ELEM_LEFT(e)  (e)->rect.x
#define ELEM_RIGHT(e) (e)->rect.x + (e)->rect.w
#define ELEM_TOP(e)   (e)->rect.y
#define ELEM_BOT(e)   (e)->rect.y + (e)->rect.h

#define ELEMS_COLLIDING(object, static_object)          \
    ELEM_RIGHT(object) >= ELEM_LEFT(static_object)  &&  \
    ELEM_LEFT(object)  <= ELEM_RIGHT(static_object) &&  \
    ELEM_BOT(object)   >= ELEM_TOP(static_object)   &&  \
    ELEM_TOP(object)   <= ELEM_BOT(static_object)

#define OBJ_OVERLAP_LEFT(object, static_object)  ELEM_RIGHT(object) - ELEM_LEFT(static_object)
#define OBJ_OVERLAP_RIGHT(object, static_object) ELEM_RIGHT(static_object) - ELEM_LEFT(object)
#define OBJ_OVERLAP_TOP(object, static_object)   ELEM_BOT(object) - ELEM_TOP(static_object)
#define OBJ_OVERLAP_BOT(object, static_object)   ELEM_BOT(static_object) - ELEM_TOP(object)

#define OBJ_OVERLAP_LEAST_LEFT(object, static_object)                                       \
    OBJ_OVERLAP_LEFT(object, static_object) < OBJ_OVERLAP_RIGHT(object, static_object) &&   \
        OBJ_OVERLAP_LEFT(object, static_object) < OBJ_OVERLAP_TOP(object, static_object) && \
        OBJ_OVERLAP_LEFT(object, static_object) < OBJ_OVERLAP_BOT(object, static_object)

#define OBJ_OVERLAP_LEAST_RIGHT(object, static_object)                                       \
    OBJ_OVERLAP_RIGHT(object, static_object) < OBJ_OVERLAP_LEFT(object, static_object) &&    \
        OBJ_OVERLAP_RIGHT(object, static_object) < OBJ_OVERLAP_TOP(object, static_object) && \
        OBJ_OVERLAP_RIGHT(object, static_object) < OBJ_OVERLAP_BOT(object, static_object)

#define OBJ_OVERLAP_LEAST_TOP(object, static_object)                                        \
    OBJ_OVERLAP_TOP(object, static_object) < OBJ_OVERLAP_BOT(object, static_object) &&      \
        OBJ_OVERLAP_TOP(object, static_object) < OBJ_OVERLAP_LEFT(object, static_object) && \
        OBJ_OVERLAP_TOP(object, static_object) < OBJ_OVERLAP_RIGHT(object, static_object)

#define OBJ_OVERLAP_LEAST_BOT(object, static_object)                                        \
    OBJ_OVERLAP_BOT(object, static_object) < OBJ_OVERLAP_TOP(object, static_object) &&      \
        OBJ_OVERLAP_BOT(object, static_object) < OBJ_OVERLAP_LEFT(object, static_object) && \
        OBJ_OVERLAP_BOT(object, static_object) < OBJ_OVERLAP_RIGHT(object, static_object)

#define APP                    app::instance()
#define EVENT_HANDLER          event_handler::instance()
#define KEY_HANDLER            key_handler::Instance()
