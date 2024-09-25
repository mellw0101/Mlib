#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <functional>

#include "../Vector.h"
#include "../def.h"
#include "../Attributes.h"

constexpr unsigned
event_to_index(const unsigned type)
{
    switch (type)
    {
        /*-< Application events >-*/
        case SDL_EVENT_QUIT :
            return 0;

        case SDL_EVENT_TERMINATING :
            return 1;

        case SDL_EVENT_LOW_MEMORY :
            return 2;

        case SDL_EVENT_WILL_ENTER_BACKGROUND :
            return 3;

        /*-<< These application events have special meaning on iOS.  README-ios.md for
         * details. >>-*/
        case SDL_EVENT_DID_ENTER_BACKGROUND :
            return 4;

        case SDL_EVENT_WILL_ENTER_FOREGROUND :
            return 5;

        case SDL_EVENT_DID_ENTER_FOREGROUND :
            return 6;

        case SDL_EVENT_LOCALE_CHANGED :
            return 7;

        case SDL_EVENT_SYSTEM_THEME_CHANGED :
            return 8;

        case SDL_EVENT_DISPLAY_ORIENTATION :
            return 9;

        case SDL_EVENT_DISPLAY_ADDED :
            return 10;

        /*-<< Keyboard events >>-*/
        case SDL_EVENT_DISPLAY_REMOVED :
            return 11;

        case SDL_EVENT_DISPLAY_MOVED :
            return 12;

        case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED :
            return 13;

        case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED :
            return 14;

        case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED :
            return 15;

        case SDL_EVENT_WINDOW_SHOWN :
            return 16;

        /*-< Mouse events >-*/
        case SDL_EVENT_WINDOW_HIDDEN :
            return 17;

        case SDL_EVENT_WINDOW_EXPOSED :
            return 18;

        case SDL_EVENT_WINDOW_MOVED :
            return 19;

        case SDL_EVENT_WINDOW_RESIZED :
            return 20;

        /* Joystick events */
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED :
            return 21;

        case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED :
            return 22;

        case SDL_EVENT_WINDOW_MINIMIZED :
            return 23;

        case SDL_EVENT_WINDOW_MAXIMIZED :
            return 24;

        case SDL_EVENT_WINDOW_RESTORED :
            return 25;

        case SDL_EVENT_WINDOW_MOUSE_ENTER :
            return 26;

        case SDL_EVENT_WINDOW_MOUSE_LEAVE :
            return 27;

        case SDL_EVENT_WINDOW_FOCUS_GAINED :
            return 28;

        case SDL_EVENT_WINDOW_FOCUS_LOST :
            return 29;

        case SDL_EVENT_WINDOW_CLOSE_REQUESTED :
            return 30;

        case SDL_EVENT_WINDOW_HIT_TEST :
            return 31;

        case SDL_EVENT_WINDOW_ICCPROF_CHANGED :
            return 32;

        case SDL_EVENT_WINDOW_DISPLAY_CHANGED :
            return 33;

        case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED :
            return 34;

        case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED :
            return 35;

        case SDL_EVENT_WINDOW_OCCLUDED :
            return 36;

        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN :
            return 37;

        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN :
            return 38;

        case SDL_EVENT_WINDOW_DESTROYED :
            return 39;

        case SDL_EVENT_WINDOW_HDR_STATE_CHANGED :
            return 40;

        /*-< Touch events >-*/
        case SDL_EVENT_KEY_DOWN :
            return 41;

        case SDL_EVENT_KEY_UP :
            return 42;

        case SDL_EVENT_TEXT_EDITING :
            return 43;

        case SDL_EVENT_TEXT_INPUT :
            return 44;

        case SDL_EVENT_KEYMAP_CHANGED :
            return 45;

        case SDL_EVENT_KEYBOARD_ADDED :
            return 46;

        case SDL_EVENT_KEYBOARD_REMOVED :
            return 47;

        case SDL_EVENT_TEXT_EDITING_CANDIDATES :
            return 48;

        case SDL_EVENT_MOUSE_MOTION :
            return 49;

        case SDL_EVENT_MOUSE_BUTTON_DOWN :
            return 50;

        case SDL_EVENT_MOUSE_BUTTON_UP :
            return 51;

        case SDL_EVENT_MOUSE_WHEEL :
            return 52;

        case SDL_EVENT_MOUSE_ADDED :
            return 53;

        case SDL_EVENT_MOUSE_REMOVED :
            return 54;

        case SDL_EVENT_JOYSTICK_AXIS_MOTION :
            return 55;

        case SDL_EVENT_JOYSTICK_BALL_MOTION :
            return 56;

        case SDL_EVENT_JOYSTICK_HAT_MOTION :
            return 57;

        case SDL_EVENT_JOYSTICK_BUTTON_DOWN :
            return 58;

        case SDL_EVENT_JOYSTICK_BUTTON_UP :
            return 59;

        case SDL_EVENT_JOYSTICK_ADDED :
            return 60;

        case SDL_EVENT_JOYSTICK_REMOVED :
            return 61;

        case SDL_EVENT_JOYSTICK_BATTERY_UPDATED :
            return 62;

        case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE :
            return 63;

        case SDL_EVENT_GAMEPAD_AXIS_MOTION :
            return 64;

        case SDL_EVENT_GAMEPAD_BUTTON_DOWN :
            return 65;

        case SDL_EVENT_GAMEPAD_BUTTON_UP :
            return 66;

        case SDL_EVENT_GAMEPAD_ADDED :
            return 67;

        case SDL_EVENT_GAMEPAD_REMOVED :
            return 68;

        case SDL_EVENT_GAMEPAD_REMAPPED :
            return 69;

        case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN :
            return 70;

        case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION :
            return 71;

        case SDL_EVENT_GAMEPAD_TOUCHPAD_UP :
            return 72;

        case SDL_EVENT_GAMEPAD_SENSOR_UPDATE :
            return 73;

        case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE :
            return 74;

        case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED :
            return 75;
        case SDL_EVENT_FINGER_DOWN :
            return 76;

        case SDL_EVENT_FINGER_UP :
            return 77;

        case SDL_EVENT_FINGER_MOTION :
            return 78;

        case SDL_EVENT_CLIPBOARD_UPDATE :
            return 79;

        case SDL_EVENT_DROP_FILE :
            return 80;

        case SDL_EVENT_DROP_TEXT :
            return 81;

        case SDL_EVENT_DROP_BEGIN :
            return 82;

        case SDL_EVENT_DROP_COMPLETE :
            return 83;

        case SDL_EVENT_DROP_POSITION :
            return 84;

        case SDL_EVENT_AUDIO_DEVICE_ADDED :
            return 85;

        case SDL_EVENT_AUDIO_DEVICE_REMOVED :
            return 86;

        case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED :
            return 87;

        case SDL_EVENT_SENSOR_UPDATE :
            return 88;

        case SDL_EVENT_PEN_PROXIMITY_IN :
            return 89;

        case SDL_EVENT_PEN_PROXIMITY_OUT :
            return 90;

        case SDL_EVENT_PEN_DOWN :
            return 91;

        case SDL_EVENT_PEN_UP :
            return 92;

        case SDL_EVENT_PEN_BUTTON_DOWN :
            return 93;

        case SDL_EVENT_PEN_BUTTON_UP :
            return 94;

        case SDL_EVENT_PEN_MOTION :
            return 95;

        case SDL_EVENT_PEN_AXIS :
            return 99;

        case SDL_EVENT_CAMERA_DEVICE_ADDED :
            return 100;

        case SDL_EVENT_CAMERA_DEVICE_REMOVED :
            return 101;

        case SDL_EVENT_CAMERA_DEVICE_APPROVED :
            return 102;

        case SDL_EVENT_CAMERA_DEVICE_DENIED :
            return 103;

        case SDL_EVENT_RENDER_TARGETS_RESET :
            return 104;

        case SDL_EVENT_RENDER_DEVICE_RESET :
            return 105;

        case SDL_EVENT_POLL_SENTINEL :
            return 106;

        case SDL_EVENT_USER :
            return 107;

        case SDL_EVENT_LAST :
            return 108;

        case SDL_EVENT_ENUM_PADDING :
        default :
            return (unsigned int)-1;
    }
    return (unsigned int)-1;
}

#define TOTAL_EVENTS 107

using event_callback_t = std::function<void(SDL_Event ev)>;
using event_vector_t   = MVector<event_callback_t>;

class event_handler {
private:
    static event_handler *_instance;
    event_vector_t          _map[TOTAL_EVENTS] {};
    SDL_Event               _ev {};

    DEL_CM_CONSTRUCTORS(event_handler);
    event_handler(void) = default;

public:
    static event_handler *instance(void) noexcept;

    template <typename F>
    __inline__ void __attr(__always_inline__, __nodebug__, __nothrow__)
    event_action(const unsigned int ev_t, F &&f) noexcept
    {
        const unsigned int type = event_to_index(ev_t);
        if (type == (unsigned int)-1)
        {
            return;
        }
        _map[type].push_back(std::forward<F>(f));
    }
    
    void __no_debug __no_throw handle_event(void) noexcept;
};
