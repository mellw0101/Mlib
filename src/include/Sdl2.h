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

    /* This class represents the state of an object.  The state is a bit field. */
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

    /* This class represents a 2D vector ( x ( float ), y ( float ) ). */
    class Vec2D
    {
    public:
        double x, y;

        /* Initializes the vector to (0, 0). */
        Vec2D(void)
            : x(0)
            , y(0)
        {}

        /* Initializes the vector to (x, y). */
        Vec2D(double x, double y)
            : x(x)
            , y(y)
        {}

        /* Add two vectors together */
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
        operator*(double const scalar) const
        {
            return Vec2D(x * scalar, y * scalar);
        }

        double
        length() const
        {
            return sqrt(x * x + y * y);
        }

        Vec2D
        normalized() const
        {
            double len = length();
            return Vec2D(x / len, y / len);
        }

        double
        dot(const Vec2D &other) const
        {
            return x * other.x + y * other.y;
        }
    };

    double static constexpr FPS          = 240.0;
    double static constexpr timePerSec   = 1.0;
    double static constexpr timePerFrame = timePerSec / FPS;
    double static constexpr GRAVITY      = 9.81;
    /* This is the gravity acceleration vector. */
    static Vec2D const GravityVec     = Vec2D(0.0, GRAVITY);
    static Vec2D const velocityChange = GravityVec * timePerFrame;

    /* This struct holds the data of Object2D. */
    typedef struct ObjectData2D
    {
        /* Position of the object ( x (float), y (float)) in the 2D plane */
        Vec2D position;
        /* Velocity of the object ( x (float), y (float)) in the 2D plane */
        Vec2D  velocity;
        int    w;
        int    h;
        double speed;
        /* State of the object, This is a bit field */
        unsigned int state;
    } ObjectData2D;

    /* This struct represents a 2D object. */
    typedef struct Object2D
    {
        /* Holds all data about the object */
        ObjectData2D data;

        /* Initialize the object with the given data.  This function is rarely used. */
        void init(const ObjectData2D &data);
        /* The velocity vector to move the object by. */
        void move(Vec2D vel);
        /* The renderer to draw the object on. */
        void draw(SDL_Renderer *renderer) const;
        /* Check if the object is static. */
        bool isStatic(void) const;
        /* Check if the object is colliding with the given object. */
        bool isColiding(const Object2D &object) const;
        /* Get the rect of the object. */
        SDL_Rect rect(void) const;
        /* Get the frect of the object. */
        SDL_FRect frect() const;
        /* Get the state of the object. */
        u32 state(u32 stateToCheck = 0) const noexcept;
        /* Update the velocity of the object. */
        void updateVelocity(void);
        /* Handle the object when it is on a static object. */
        void onStaticObjCollision(Object2D const &obj, Vec2D velVecToApply);
    } Object2D;

    using KeyMap = unordered_map<unsigned char, vector<function<void()>>>;

    /* This Class Represents The Keyboard Object, it Is Used To Handle Keyboard Events
     * It Is The Interface Between The Keyboard And the Engine
     * Note This Class Is A Singleton Class, And Can Only Be Accessed Through The 'Instance'
     * Function.  This Class Is Used To Add Actions For Keys It Runs Every Frame And Executes All Lambda
     * Functions Added Using The 'addActionForKey' Function */
    class KeyObject
    {
    private:
        KeyMap            keymap;
        static KeyObject *KeyObjectInstance;

    public:
        /* This function returns the instance of the KeyObject.
         * If the instance Does Not Yet Exist, It Creates One.
         * This Function Is The Only Way To Get The Instance Of The KeyObject. */
        static KeyObject *
        Instance(void)
        {
            if (!KeyObjectInstance)
            {
                KeyObjectInstance = new KeyObject();
            }
            return KeyObjectInstance;
        }

        /* This function adds an action for a key, using perfect forwarding. */
        template <typename F, typename... Args>
        void
        addActionForKey(unsigned char key, F &&func, Args &&...args)
        {
            auto boundFunc = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
            keymap[key].emplace_back(
                [boundFunc]()
                {
                    boundFunc();
                });
        }

        /* This function handles the key events.  Only called from function 'run'
         * in 'Mlib::Sdl2::Core' class.  This function is called every frame.
         * Note that this function runs all lambdas for each key that is pressed. */
        void
        handleKeyEvent(void)
        {
            const unsigned char *state = SDL_GetKeyboardState(nullptr);
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
