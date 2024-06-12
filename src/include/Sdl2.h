#pragma once

#include <SDL2/SDL.h>
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "def.h"

namespace Mlib::Sdl2 {
    extern int SCREEN_WIDTH;
    extern int SCREEN_HEIGHT;

    class Direction
    {
    public:
        enum
        {
            UP    = 1 << 0,
            DOWN  = 1 << 1,
            LEFT  = 1 << 2,
            RIGHT = 1 << 3,
        };
    };

    class State
    {
    public:
        enum
        {
            STATIC    = 1 << 0,
            ON_GROUND = 1 << 1,
            IS_PLAYER = 1 << 2
        };
    };

    class Vec2D
    {
    public:
        f32 x, y;

        // Constructors
        Vec2D()
            : x(0)
            , y(0)
        {}

        Vec2D(f32 x, f32 y)
            : x(x)
            , y(y)
        {}

        Vec2D(s32 x, s32 y)
            : x(static_cast<f32>(x))
            , y(static_cast<f32>(y))
        {}

        Vec2D(pair<f32, f32> xY) // Initialize the vector with a pair of floats
            : x(xY.first)
            , y(xY.second)
        {}

        Vec2D(pair<s32, s32> xY) // Initialize the vector with a pair of integers
            : x(static_cast<f32>(xY.first))
            , y(static_cast<f32>(xY.second))
        {}

        // Add another vector to this one
        Vec2D&
        operator+=(const Vec2D& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        // Multiply this vector by a scalar
        Vec2D&
        operator*=(f32 scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        SDL_FPoint
        toSDL_FPoint() const
        {
            return {x, y};
        }

        // Retrieve the integer x component
        s32
        getXInt() const
        {
            return static_cast<s32>(x);
        }

        // Retrieve the integer y component
        s32
        getYInt() const
        {
            return static_cast<s32>(y);
        }

        operator pair<f32, f32>() const
        {
            return {x, y};
        }
    };

    /**
        @struct ObjectData
        @brief This struct holds the data of the object Data For 2D.
        */
    typedef struct ObjectData2D
    {
        Vec2D position; // position,  Coordinate Of The Object
        s32   w;
        s32   h;
        f32   speed;

        /**
            @brief The state of the engine (running, paused, etc.) Using Only Bitwise Operations
            @note if (state & (1 << 0)) != 0, then the engine is running
            */
        u32 state;
    } ObjectData2D;

    /**
        @struct Object
        @brief This struct holds the object.
        */
    typedef struct Object2D
    {
        ObjectData2D data;

        auto init(const ObjectData2D& data) -> void;
        auto move(s32 Direction, f32 speed_override = 0.0) -> void;
        auto move(Vec2D vel) -> void;
        auto draw(SDL_Renderer* renderer) const -> void;
        auto isStatic() const -> bool;
        auto rect() const -> SDL_Rect;
        auto frect() const -> SDL_FRect;
        auto state() const -> u32;
    } Object2D;

    /**
        @brief This map holds the keys and vector of functions.
        */
    typedef unordered_map<u8, vector<function<void()>>> KeyMap;

    /**
        @struct KeyObject
        @brief This struct holds the keymap.
        */
    class KeyObject
    {
    private:
        KeyMap            keymap;
        static KeyObject* KeyObjectInstance;

    public:
        /**
            @b Function: @c 'Instance'
            @return @c KeyObject*
            @brief:
                @note This function returns the instance of the KeyObject.
                @note If the instance Does Not Yet Exist, It Creates One.
                @note This Function Is The Only Way To Get The Instance Of The KeyObject.
            */
        static KeyObject*
        Instance()
        {
            if (!KeyObjectInstance)
            {
                KeyObjectInstance = new KeyObject();
            }
            return KeyObjectInstance;
        }

        /**
            @b Function: @c 'addActionForKey'
                @return void
                @brief:
                    @note This function adds an action for a key.
                    @note The function is called when the key is pressed.
            */
        template <typename F, typename... Args>
        void
        addActionForKey(u8 key, F&& func, Args&&... args)
        {
            auto boundFunc = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
            keymap[key].emplace_back(
                [boundFunc]()
                {
                    boundFunc();
                });
        }

        /**
            @b Function: @c 'handleKeyEvent'
                @return void
                @brief:
                    @note This function handles the key event.
                    @note Only Called From Func 'run' In 'Engine::Base' Class
                    @note This Function Is Called Every Frame
            */
        void
        handleKeyEvent()
        {
            const u8* state = SDL_GetKeyboardState(nullptr);
            for (auto const& [key, funcs] : keymap)
            {
                if (state[key])
                {
                    for (auto const& func : funcs)
                    {
                        func();
                    }
                }
            }
        }
    };

    /**
       @class @b 'Core'
       @return @c
       @brief Base class To Control The Engine
       */
    class Core
    {
    private:
        string window_title;
        u32    frames;
        /**
            @brief The state of the engine (running, paused, etc.) Using Only Bitwise Operations
            @note if (state & (1 << 0)) != 0, then the engine is running
            */
        u32              state;
        SDL_Window*      window   = nullptr;
        SDL_Renderer*    renderer = nullptr;
        SDL_Event        event;
        auto             init() -> int;
        vector<Object2D> objects;
        bool             running = true;
        static Core*     CoreInstance;

    public:
        auto run() -> int;
        auto createObject(const Object2D& object) -> void;
        static Core*
        Inst(const string& window_title, const int& window_width, const int& window_height)
        {
            if (CoreInstance == nullptr)
            {
                lock_guard<mutex> lock(mutex);
                if (CoreInstance == nullptr)
                {
                    CoreInstance = new Core {window_title, window_width, window_height};
                }
            }
            return CoreInstance;
        }
        void shutdown();
        auto cleanup() -> void;
        auto applyPhysics() -> void;
        auto logic() -> void;
        auto clear() -> void;
        auto draw() -> void;
        auto update() -> void;
        auto pollForEvents() -> void;
        auto initSDL() -> int;
        auto createWindow() -> int;
        auto createRenderer() -> int;
        auto setupKeys() -> void;
        auto stop() -> void;
        auto getObjects() -> vector<Object2D>&;

        Core(const string& window_title, const int& window_width, const int& window_height);
    };
} // namespace Mlib::Sdl2
