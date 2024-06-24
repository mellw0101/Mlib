#pragma once

#if defined(__i386__) || defined(__x86_64__)

#    include <SDL2/SDL.h>
#    include <SDL2/SDL_rect.h>
#    include <functional>
#    include <map>
#    include <mutex>
#    include <string>
#    include <unordered_map>
#    include <vector>

#    include "def.h"
using namespace std;


namespace Mlib::Sdl2
{
    extern int SCREEN_WIDTH;
    extern int SCREEN_HEIGHT;

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
    /// @brief
    /// - This class represents a 2D vector ( x ( float ), y ( float ) ).
    class Vec2D
    {
    public:
        f64 x, y;

        /// @name Default Constructor
        /// @brief
        /// - Initializes the vector to ( 0, 0 )
        Vec2D()
            : x(0)
            , y(0)
        {}

        /// @name Constructor
        /// @brief
        /// - Initializes the vector to ( x, y )
        /// @param x ( float )
        /// - The x component of the vector
        /// @param y ( float )
        /// - The y component of the vector
        Vec2D(f64 x, f64 y)
            : x(x)
            , y(y)
        {}

        /// @name operator+=
        /// @brief
        /// - Add two vectors together
        /// @param other ( Vec2D const& )
        /// - The other vector to add to this vector
        /// @returns Vec2D&
        /// - reference to this vector after adding the other vector
        Vec2D &
        operator+=(const Vec2D &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vec2D &
        operator-=(const Vec2D &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vec2D
        operator*(f64 const scalar) const
        {
            return Vec2D(x * scalar, y * scalar);
        }

        f64
        length() const
        {
            return sqrt(x * x + y * y);
        }

        Vec2D
        normalized() const
        {
            f64 len = length();
            return Vec2D(x / len, y / len);
        }

        f64
        dot(const Vec2D &other) const
        {
            return x * other.x + y * other.y;
        }
    };

    f64 static constexpr FPS          = 240.0;
    f64 static constexpr timePerSec   = 1.0;
    f64 static constexpr timePerFrame = timePerSec / FPS;
    f64 static constexpr GRAVITY      = 9.81;
    /// @name GravityVec
    /// @brief
    /// - This is the gravity acceleration vector.
    static Vec2D const GravityVec     = Vec2D(0.0, GRAVITY);
    static Vec2D const velocityChange = GravityVec * timePerFrame;

    /// @name ObjectData
    /// TYPE: @struct
    /// @brief
    /// - This struct holds the data of Object2D.
    typedef struct ObjectData2D
    {
        /// @name position
        /// @brief
        /// - Position of the object ( x (float), y (float)) in the 2D plane
        Vec2D position;

        /// @name velocity
        /// @brief
        /// - Velocity of the object ( x (float), y (float)) in the 2D plane
        Vec2D velocity;

        s32 w;
        s32 h;
        f64 speed;

        /// @brief State of the object
        /// @note This is a bit field
        u32 state;
    } ObjectData2D;

    /// @name Object2D
    /// TYPE: @struct
    /// @brief
    /// - This struct represents a 2D object.
    typedef struct Object2D
    {
        /// @name data
        /// @brief The data of the object.
        /// - Holds all data about the object
        ObjectData2D data;

        /// @name init
        /// @brief
        /// - Initialize the object with the given data.
        /// @param data
        /// - The data to initialize the object with.
        /// @note
        /// - This function is rarely used.
        /// @returns void
        void init(const ObjectData2D &data);

        /// @name move
        /// @brief
        /// - Move Object By The Given Velocity Vector
        /// @param vel
        /// - The velocity vector to move the object by.
        /// @returns void
        void move(Vec2D vel);

        /// @name draw
        /// @brief
        /// - Draw the object on the screen.
        /// @param renderer
        /// - The renderer to draw the object on.
        /// @returns void
        void draw(SDL_Renderer *renderer) const;

        /// @name isStatic
        /// @brief
        /// - Check if the object is static.
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
        bool isColiding(const Object2D &object) const;

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
        u32 state(u32 stateToCheck = 0) const noexcept;

        /// @name updateVelocity
        /// @brief
        /// - Update the velocity of the object.
        /// @returns void
        void updateVelocity();

        /// @name onStaticObj
        /// @brief
        /// - Handle the object when it is on a static object.
        /// @param obj
        /// - The object to handle.
        /// @param velVecToApply
        /// - The velocity vector to apply.
        /// @returns void
        void onStaticObjCollision(Object2D const &obj, Vec2D velVecToApply);
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
        static KeyObject *KeyObjectInstance;

    public:
        /// @name Instance
        /// @brief
        /// - This function returns the instance of the KeyObject.
        /// - If the instance Does Not Yet Exist, It Creates One.
        /// - This Function Is The Only Way To Get The Instance Of The KeyObject.
        /// @returns KeyObject*
        static KeyObject *
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
        addActionForKey(u8 key, F &&func, Args &&...args)
        {
            auto boundFunc = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
            keymap[key].emplace_back(
                [boundFunc]()
                {
                    boundFunc();
                });
        }

        /// @name handleKeyEvent
        /// @brief
        /// - This function handles the key events.
        /// - Only called from function 'run' in 'Mlib::Sdl2::Core' class.
        /// - This function is called every frame.
        /// @note
        /// - This function runs all lambdas for each key that is pressed.
        /// @return void
        void
        handleKeyEvent()
        {
            const u8 *state = SDL_GetKeyboardState(nullptr);
            for (auto const &[key, funcs] : keymap)
            {
                if (state[key])
                {
                    for (auto const &func : funcs)
                    {
                        func();
                    }
                }
            }
        }
    };

    /// @class Core
    /// @brief
    /// - This Is The Main Class Of The Engine.
    /// - It Handles The Creation Of Objects
    /// - as well as the main loop.
    /// - This Class Is A Singleton Class And Can
    /// - Only Be Accessed Through The 'Instance' Function.
    class Core
    {
    private:
        string       window_title;
        static Core *CoreInstance;

        SDL_Window        *window   = nullptr;
        SDL_Renderer      *renderer = nullptr;
        function<void()>   mainLoop = nullptr;
        bool               running  = true;
        u32                frames;
        SDL_Event          event;
        vector<Object2D *> objects;

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
        void createObject(const Object2D &object);

        /// @brief Return The Instance Of The Core Class
        /// - If The Instance Does Not Exist,
        /// - It Creates One
        /// - This Function Is The Only Way To Get
        /// - The Instance Of The Core Class
        /// @param window_title The title of the window
        /// @param window_width The width of the window
        /// @param window_height The height of the window
        /// @returns Core*
        static Core *Instance();

        /// @brief Cleanup used to clean up on exit
        /// @returns void
        void cleanup();

        template <typename CallBack>
        void
        setMainLoop(CallBack &&callBack)
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
        u32  getCurrentFrameCount() const noexcept;
        u32 *getCurrentFrameCountPtr() noexcept;
        void setTitle(const string &title);

        vector<Object2D *> &getObjects();
        Core() = default;
    };
} // namespace Mlib::Sdl2

#endif
