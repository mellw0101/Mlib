#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
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

    /// @class State
    /// @brief This class represents the state of an object.
    /// - The state is a bit field.
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

    /// @class Vec2D
    /// @brief This class represents a 2D vector ( x ( float ), y ( float ) ).
    class Vec2D
    {
    public:
        f32 x, y;

        /// @brief Default Constructor
        /// @note Initializes the vector to ( 0, 0 )
        Vec2D()
            : x(0)
            , y(0)
        {}

        /// @brief Constructor
        /// @param x ( float ) The x component of the vector
        /// @param y ( float ) The y component of the vector
        /// @note Initializes the vector to ( x, y )
        Vec2D(f32 x, f32 y)
            : x(x)
            , y(y)
        {}

        /// @brief Constructor
        /// @param x The x component of the vector (integer)
        /// @param y The y component of the vector (integer)
        /// @note Initializes the vector to (x, y)
        Vec2D(s32 x, s32 y)
            : x(static_cast<f32>(x))
            , y(static_cast<f32>(y))
        {}

        /// @brief Constructor
        /// @param xY is a pair<float, float> (x, y)
        /// @note Initializes the vector to (x, y)
        Vec2D(pair<f32, f32> xY)
            : x(xY.first)
            , y(xY.second)
        {}

        /// @brief Constructor
        /// @param xY is a pair<int, int> (x, y)
        /// @note Initializes the vector to (x, y)
        Vec2D(pair<s32, s32> xY)
            : x(static_cast<f32>(xY.first))
            , y(static_cast<f32>(xY.second))
        {}

        /// @brief Add two vectors together
        /// @param other The other vector to add to this vector
        /// @returns Vec2D&
        /// - reference to this vector after adding the other vector
        Vec2D&
        operator+=(const Vec2D& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

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

    /// @struct ObjectData
    /// @brief This struct holds the data of Object2D.
    typedef struct ObjectData2D
    {
        /// @brief Position of the object (x, y) in the 2D plane
        /// @note The position is a vector of floats
        Vec2D position;

        s32 w;
        s32 h;
        f32 speed;

        /// @brief State of the object
        /// @note This is a bit field
        u32 state;
    } ObjectData2D;

    /// @struct Object2D
    /// @brief This struct represents a 2D object.
    typedef struct Object2D
    {
        /// @name data
        /// @brief The data of the object.
        /// - Holds all data about the object
        ObjectData2D data;

        /// @name init
        /// @brief Initialize the object with the given data.
        /// @param data The data to initialize the object with.
        /// @note This function is rarely used.
        void init(const ObjectData2D& data);

        /// @name move
        /// @brief Move Object By The Given Velocity Vector
        /// @param vel The velocity vector to move the object by.
        /// @returns void
        void move(Vec2D vel);

        /// @name draw
        /// @brief Draw the object on the screen.
        /// @param renderer The renderer to draw the object on.
        /// @returns void
        void draw(SDL_Renderer* renderer) const;

        /// @name isStatic
        /// @brief Check if the object is static.
        /// @returns bool
        /// - true  (object is static).
        /// - false (object is not static).
        bool isStatic() const;

        /// @name isColiding
        /// @brief Check if the object is colliding.
        /// - with the given object.
        /// @returns bool
        /// - true  (object is on the ground).
        /// - false (object is not on the ground).
        bool isColiding(const Object2D& object) const;

        /// @name rect
        /// @brief Get the rect of the object.
        /// @returns SDL_Rect (x, y, w, h) (integer)
        SDL_Rect rect() const;

        /// @name frect
        /// @brief Get the frect of the object.
        /// @returns SDL_FRect (x, y, w, h) (float)
        SDL_FRect frect() const;

        /// @name state
        /// @brief Get the state of the object.
        /// @returns u32 (state of the object as a bit field)
        /// @note
        /// - This func is marked as const because it
        /// - does not modify the object and is only
        /// - a trivial accessor for the state.
        u32 state() const;
    } Object2D;

    using KeyMap = unordered_map<u8, vector<function<void()>>>;

    /// @class KeyObject
    /// @brief This Class Represents The Keyboard Object
    /// - This Class Is Used To Handle Keyboard Events
    /// - It Is The Interface Between The Keyboard And
    /// - The Engine
    /// @note This Class Is A Singleton Class,
    /// - And Can Only Be Accessed Through The 'Instance'
    /// - Function
    /// - This Class Is Used To Add Actions For Keys
    /// - It Runs Every Frame And Executes All Lambda
    /// - Functions Added Using The 'addActionForKey' Function
    class KeyObject
    {
    private:
        KeyMap            keymap;
        static KeyObject* KeyObjectInstance;

    public:
        /// @name Instance
        /// @brief
        /// - This function returns the instance of the KeyObject.
        /// - If the instance Does Not Yet Exist, It Creates One.
        /// - This Function Is The Only Way To Get The Instance Of The KeyObject.
        /// @returns KeyObject*
        static KeyObject*
        Instance()
        {
            if (!KeyObjectInstance)
            {
                KeyObjectInstance = new KeyObject();
            }
            return KeyObjectInstance;
        }

        /// @brief addActionForKey
        /// @return void
        /// @note This function adds an action for a key,
        /// - using perfect forwarding.
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

    /// @class Core
    /// @brief Core Class Of The Engine.
    /// @note This Is The Main Class Of The Engine.
    /// - It Handles The Creation Of Objects.
    /// - This Class Is A Singleton Class And Can
    /// - Only Be Accessed Through The 'Inst' Function.
    class Core
    {
    private:
        string window_title;
        u32    frames;

        /// @brief The state of the engine (running, paused, etc.) Using Only Bitwise Operations
        /// @note if (state & (1 << 0)) != 0, then the engine is running
        u32 state;

        SDL_Window*      window   = nullptr;
        SDL_Renderer*    renderer = nullptr;
        SDL_Event        event;
        vector<Object2D> objects;
        bool             running = true;
        static Core*     CoreInstance;
        function<void()> mainLoop = nullptr;

        auto init() -> int;

    public:
        /// @brief Main Loop Of The Engine
        /// @returns int
        /// - 0 (success)
        /// - 1 (failure)
        int run();

        /// @brief Create An Object
        /// @param object The object to create
        /// @returns void
        void createObject(const Object2D& object);

        /// @brief Return The Instance Of The Core Class
        /// - If The Instance Does Not Exist,
        /// - It Creates One
        /// - This Function Is The Only Way To Get
        /// - The Instance Of The Core Class
        /// @param window_title The title of the window
        /// @param window_width The width of the window
        /// @param window_height The height of the window
        /// @returns Core*
        static Core* Instance(const string& window_title, int window_width, int window_height);

        /// @brief Cleanup used to clean up on exit
        /// @returns void
        void cleanup();

        template <typename CallBack>
        void
        setMainLoop(CallBack&& callBack)
        {
            mainLoop = std::forward<CallBack>(callBack);
        }

        void applyPhysics();
        void logic();
        void clear();
        void draw();
        void update();
        void pollForEvents();
        int  initSDL();
        int  createWindow();
        int  createRenderer();
        void setupKeys();
        void stop();

        vector<Object2D>& getObjects();

        Core(const string& window_title, int window_width, int window_height);
    };
} // namespace Mlib::Sdl2
