#pragma once

#if defined(__i386__) || defined(__x86_64__)

#    include "Debug.h"
#    include "Profile.h"

#    include <SDL2/SDL.h>
#    include <SDL2/SDL_rect.h>
#    include <SDL2/SDL_ttf.h>

#    ifdef __cplusplus
#        include <functional>
#        include <map>
#        include <mutex>
#        include <string>
#        include <unordered_map>
#        include <vector>
#        include "Vector.h"
#        include "def.h"

using std::function;
using std::string;

namespace Mlib::Sdl2
{
    using std::lock_guard;
    using std::mutex;
    using std::unordered_map;
    using std::vector;

    extern int SCREEN_WIDTH;
    extern int SCREEN_HEIGHT;

    /* This class represents the state of an object.  The state is a bit field.
     */
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
        Vec2D &operator+=(const Vec2D &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vec2D &operator-=(const Vec2D &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vec2D operator*(double const scalar) const
        {
            return Vec2D(x * scalar, y * scalar);
        }

        double length() const
        {
            return sqrt(x * x + y * y);
        }

        Vec2D normalized() const
        {
            double len = length();
            return Vec2D(x / len, y / len);
        }

        double dot(const Vec2D &other) const
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

        /* Initialize the object with the given data.  This function is rarely
         * used. */
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
    /* This Class Represents The Keyboard Object, it Is Used To Handle Keyboard
     * Events It Is The Interface Between The Keyboard And the Engine Note This
     * Class Is A Singleton Class, And Can Only Be Accessed Through The
     * 'Instance' Function.  This Class Is Used To Add Actions For Keys It Runs
     * Every Frame And Executes All Lambda Functions Added Using The
     * 'addActionForKey' Function */
    class KeyObject
    {
    private:
        KeyMap            keymap;
        static KeyObject *KeyObjectInstance;

    public:
        /* This function returns the instance of the KeyObject.
         * If the instance Does Not Yet Exist, It Creates One.
         * This Function Is The Only Way To Get The Instance Of The KeyObject. */
        static KeyObject *Instance(void)
        {
            if (!KeyObjectInstance)
            {
                KeyObjectInstance = new KeyObject();
            }
            return KeyObjectInstance;
        }

        /* This function adds an action for a key, using perfect forwarding. */
        template <typename F, typename... Args>
        void addActionForKey(unsigned char key, F &&func, Args &&...args)
        {
            auto boundFunc = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
            keymap[key].emplace_back([boundFunc]() {
                boundFunc();
            });
        }

        /* This function handles the key events.  Only called from function
         * 'run' in 'Mlib::Sdl2::Core' class.  This function is called every
         * frame. Note that this function runs all lambdas for each key that is
         * pressed. */
        void handleKeyEvent(void)
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

    // This Is The Main Class Of The Engine.
    // It Handles The Creation Of Objects
    // as well as the main loop.
    // This Class Is A Singleton Class And Can
    // Only Be Accessed Through The 'Instance' Function.
    class Core
    {
    private:
        static Core         *CoreInstance;
        SDL_Window          *window   = nullptr;
        SDL_Renderer        *renderer = nullptr;
        SDL_Event            event;
        const char          *window_title = nullptr;
        bool                 running      = true;
        function<void(void)> mainLoop     = nullptr;
        unsigned int         frames;
        vector<Object2D *>   objects;

        void init(void);

    public:
        // Main Loop Of The Engine
        int run(void);

        // Create An Object
        void create_object(const Object2D &object);

        // Return The Instance Of The Core Class
        static Core *Instance(void);

        template <typename CallBack>
        void setMainLoop(CallBack &&callBack)
        {
            mainLoop = std::forward<CallBack>(callBack);
        }

        void          applyPhysics();
        void          clear(void);
        void          draw(void);
        void          update(void);
        void          pollForEvents(void);
        void          stop(void);
        unsigned int  getCurrentFrameCount(void) const noexcept;
        unsigned int *getCurrentFrameCountPtr(void) noexcept;
        void          set_title(const char *str);

        vector<Object2D *> &getObjects();
        Core() = default;
    };
}

namespace utils
{
    double screen_dpi(int sc_pix_w, int sc_pix_h, double sc_inch_w, double sc_inch_h);

    constexpr unsigned int event_to_index(unsigned int type)
    {
        switch (type)
        {
            /*-< Application events >-*/
            case 0x100 :
                return 0;
            case 257 :
                return 1;
            case 258 :
                return 2;
            case 259 :
                return 3;
            /*-<< These application events have special meaning on iOS.
                  README-ios.md for details. >>-*/
            case 260 :
                return 4;
            case 261 :
                return 5;
            case 262 :
                return 6;
            case 263 :
                return 7;
            case 0x150 : /* <-- Display state changed. */
                return 8;
            case 0x200 : /* <-- Window state changed. */
                return 9;
            case 513 :   /* <-- System specific event. */
                return 10;
            /*-<< Keyboard events >>-*/
            case 768 : /* <-- Key pressed. */
                return 11;
            case 769 : /* <-- Key released. */
                return 12;
            case 770 : /* <-- Keyboard text editing (composition) */
                return 13;
            case 771 : /* <-- Keyboard text input */
                return 14;
            case 772 : /* <-- Keymap changed due to a system event such as an
                              input language or keyboard layout change. */
                return 15;
            case 773 : /* <-- Extended keyboard text editing (composition) */
                return 16;
            /*-< Mouse events >-*/
            case 0x400 : /* <-- Mouse moved */
                return 17;
            case 1025 :  /* <-- Mouse button pressed */
                return 18;
            case 1026 :  /* <-- Mouse button released */
                return 19;
            case 1027 :  /* <-- Mouse wheel motion */
                return 20;
            /* Joystick events */
            case 0x600 : /* <-- Joystick axis motion */
                return 21;
            case 1537 :  /* <-- Joystick trackball motion */
                return 22;
            case 1538 :  /* <-- Joystick hat position change */
                return 23;
            case 1539 :  /* <-- Joystick button pressed */
                return 24;
            case 1540 :  /* <-- Joystick button released */
                return 25;
            case 1541 :  /* <-- A new joystick has been inserted into the system */
                return 26;
            case 1542 :  /* <-- An opened joystick has been removed */
                return 27;
            case 1543 :  /* <-- Joystick battery level change */
                return 28;
            /*-< Game controller events >-*/
            case 1616 : /* <-- Game controller axis motion */
                return 29;
            case 1617 : /* <-- Game controller button pressed */
                return 30;
            case 1618 : /* <-- Game controller button released */
                return 31;
            case 1619 : /* <-- A new Game controller has been inserted into the
                                                system */
                return 32;
            case 1620 : /* <-- An opened Game controller has been removed */
                return 33;
            case 1621 : /* <-- The controller mapping was updated */
                return 34;
            case 1622 : /* <-- Game controller touchpad was touched */
                return 35;
            case 1623 : /* <-- Game controller touchpad finger was moved */
                return 36;
            case 1624 : /* <-- Game controller touchpad finger was lifted */
                return 37;
            case 1625 : /* <-- Game controller sensor was updated */
                return 38;
            case 1626 :
                return 39;
            case 1627 : /* <-- Game controller Steam handle has changed */
                return 40;
            /*-< Touch events >-*/
            case 1792 :
                return 41;
            case 1793 :
                return 42;
            case 1794 :
                return 43;
            /*-< Gesture events >-*/
            case 2048 :
                return 44;
            case 2049 :
                return 45;
            case 2050 :
                return 46;
            /*-< Clipboard events >-*/
            case 2304 : /* <-- The clipboard or primary selection changed */
                return 47;
            /*-< Drag and drop events >-*/
            case 0x1000 : /* <-- The system requests a file open */
                return 48;
            case 4097 :   /* <-- text/plain drag-and-drop event */
                return 49;
            case 4098 :   /* <-- A new set of drops is beginning (NULL filename) */
                return 50;
            case 4099 :   /* <-- Current set of drops is now complete (NULL filename) */
                return 51;
            /*-< Audio hotplug events >-*/
            case 4352 : /* <-- A new audio device is available */
                return 52;
            case 4353 : /* <-- An audio device has been removed. */
                return 53;
            /*-< Sensor events >-*/
            case 4608 : /* <-- A sensor was updated */
                return 54;
            /*-< Render events >-*/
            case 0x2000 : /* <-- The render targets have been reset and their contents
                                               need to be updated */
                return 55;
            case 8193 :   /* <-- The device has been reset and all textures need to be
                                                recreated */
                return 56;
            /*-< Internal events >-*/
            case 0x7F00 : /* <-- Signals the end of an event poll cycle */
                return 57;
            case 0x8000 : /* Events ::SDL_USEREVENT through ::SDL_LASTEVENT are for your use,
                           * and should be allocated with SDL_RegisterEvents(). */
                return 58;
        }
        return (unsigned int)-1;
    }

    SDL_Surface *create_sdl_surface_from_bitfield(MVector<MVector<int>> data);

    struct sdl_texture_exceptions_t
    {
        static SDL_Texture *blank_texture;
    };

#        define TOTAL_EVENTS 59
    using sdl_event_callback_t = std::function<void(SDL_Event ev)>;
    using sdl_event_vector_t   = MVector<sdl_event_callback_t>;
}

namespace sdl_structs
{
    class sdl_event_handler_t
    {
        utils::sdl_event_vector_t   map[TOTAL_EVENTS] {};
        static sdl_event_handler_t *instance;
        SDL_Event                   ev;

    public:
        static sdl_event_handler_t *Instance(void);
        ~sdl_event_handler_t(void);
        template <typename F>
        void add_action_for_event(unsigned int ev_t, F &&f)
        {
            const unsigned int type = utils::event_to_index(ev_t);
            if (type == (unsigned int)-1)
            {
                return;
            }
            map[type].push_back(std::forward<F>(f));
        }
        void handle_event(void);
    };

    struct sdl_element_text_t
    {
        int          x = 0;
        int          y = 0;
        int          w;
        int          h;
        unsigned int format;
        int          access;
        SDL_Texture *texture = nullptr;
    };

    struct sdl_element_t
    {
        SDL_Color           color     = {0, 0, 0, 0};
        SDL_Rect            rect      = {0, 0, 0, 0};
        unsigned int        id        = 0;
        SDL_Renderer      **renptr    = nullptr;
        sdl_element_text_t *text_data = nullptr;
        /*-<< Methods >>-*/
        void draw(void);
        void add_text_data(int x, int y, SDL_Texture *texture);
        void render_rect(void);
    };

    class sdl_app_t
    {
        static sdl_app_t        *_instance;
        unsigned int             frames  = 0;
        int                      _width  = 600;
        int                      _height = 400;
        SDL_Window              *_window;
        SDL_Renderer            *_renderer;
        const char              *_title = nullptr;
        MVector<sdl_element_t *> _elements;
        function<void()>         main_loop = nullptr;
        bool                     _running;

        sdl_app_t(void);
        void init(void);
        void set_color(SDL_Color *) const noexcept;
        void draw_rect(SDL_Rect *) const noexcept;
        void cleanup(void);

    public:
        ~sdl_app_t(void);
        static sdl_app_t *instance(void) noexcept;
        int               run(void) noexcept;
        void              set_main_loop(std::function<void()>);
        sdl_element_t    *new_element(sdl_element_t) noexcept;
        void              quit(void) noexcept;
        TTF_Font         *retrieve_new_font(const char *, int, const char *);
        SDL_Texture      *make_text_texture(TTF_Font *font, const char *text, SDL_Color color);
        void              render_text(int x, int y, SDL_Texture *texture);
        void              move_element(unsigned int id, int x, int y);
        int               width(void) const noexcept;
        int               height(void) const noexcept;
        inline void       set_width(int w) noexcept;
        inline void       set_height(int h) noexcept;
        sdl_element_t    *element_from_id(unsigned int id);
        void              add_text_to_window(unsigned int id, TTF_Font *font, const string &text,
                                             SDL_Color color, int x, int y);
    };
};

typedef utils::sdl_texture_exceptions_t  sdl_texture_exceptions;
typedef sdl_structs::sdl_event_handler_t sdl_event_handler;
typedef sdl_structs::sdl_element_text_t  sdl_element_text;
typedef sdl_structs::sdl_element_t       sdl_element;
typedef sdl_structs::sdl_app_t           sdl_app;

#        define SDL_APP           sdl_app::instance()
#        define SDL_KEY_HANDLER   Mlib::Sdl2::KeyObject::Instance()
#        define SDL_EVENT_HANDLER sdl_event_handler::Instance()
#    endif /* __cplusplus */

#endif
