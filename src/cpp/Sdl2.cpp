/// @file Sdl2.cpp
/// @brief Contains the implementation of the Sdl2 namespace.
/// @note This file defines the implementation of the Sdl2 namespace.
#include "../include/Sdl2.h"
#include "../include/Debug.h"

#if defined(__i386__) || defined(__x86_64__)

#    include <SDL2/SDL_ttf.h>

namespace Mlib::Sdl2
{
    inline int SCREEN_WIDTH;
    inline int SCREEN_HEIGHT;

    KeyObject *KeyObject::KeyObjectInstance = nullptr;
    Core      *Core::CoreInstance           = nullptr;

    /* @struct @c Object2D Function Definitions */

    void Object2D::init(ObjectData2D const &data)
    {
        this->data = data;
    }

    void Object2D::move(Vec2D vel)
    {
        vel.x = (data.position.x + data.w + vel.x >= SCREEN_WIDTH)
                  ? SCREEN_WIDTH - data.position.x - data.w
                  : vel.x;
        vel.y = (data.position.y + data.h + vel.y >= SCREEN_HEIGHT)
                  ? SCREEN_HEIGHT - data.position.y - data.h
                  : vel.y;
        vel.x = (data.position.x + vel.x < 0) ? -data.position.x : vel.x;
        vel.y = (data.position.y + vel.y < 0) ? -data.position.y : vel.y;
        data.position += vel;
    }

    void Object2D::draw(SDL_Renderer *renderer) const
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect currentRect = rect();
        SDL_RenderFillRect(renderer, &currentRect);
    }

    bool Object2D::isStatic(void) const
    {
        return data.state & State::STATIC;
    }

    SDL_Rect Object2D::rect(void) const
    {
        SDL_Rect rect = {
            (int)data.position.x,
            (int)data.position.y,
            data.w,
            data.h,
        };
        return rect;
    }

    SDL_FRect Object2D::frect(void) const
    {
        return {
            (float)data.position.x,
            (float)data.position.y,
            (float)data.w,
            (float)data.h,
        };
    }

    unsigned int Object2D::state(unsigned int stateToCheck) const noexcept
    {
        if (stateToCheck)
        {
            return data.state & stateToCheck;
        }
        return data.state;
    }

    void Object2D::updateVelocity(void)
    {
        /* Calculate velocity change due to acceleration */
        const Vec2D velocityChange = GravityVec * timePerFrame;
        /* Update velocity */
        data.velocity += velocityChange;
        /* Update position */
        data.position += data.velocity;
    }

    void Object2D::onStaticObjCollision(Object2D const &obj, Vec2D velVecToApply)
    {
        const float X  = data.position.x;
        const float Y  = data.position.y;
        const float W  = data.w;
        const float H  = data.h;
        const float oX = obj.data.position.x;
        const float oY = obj.data.position.y;
        const float oW = obj.data.w;
        const float oH = obj.data.h;
        if (X <= oX + oW && X + W >= oX && Y <= oY + oH && Y + H >= oY)
        {
            move(velVecToApply);
        }
    }

    /* Core Function Definitions */
    void Core::create_object(const Object2D &object)
    {
        Object2D *nObj = new Object2D {object};
        objects.emplace_back(nObj);
    }

    void Core::init(void)
    {
        if (SDL_Init(SDL_INIT_VIDEO) == -1)
        {
            logE("Failed to init sdl, SDL_ERROR: ('%s').", SDL_GetError());
            exit(1);
        }
        window = SDL_CreateWindow((window_title == nullptr ? "placeholder" : window_title),
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            logE("Failed to create window, SDL_ERROR: ('%s').", SDL_GetError());
            SDL_Quit();
            exit(1);
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            logE("Failed to create renderer, SDL_ERROR: ('%s').", SDL_GetError());
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(1);
        }
    }

    int Core::run(void)
    {
        init();
        while (running)
        {
            frames++;
            clear();
            pollForEvents();
            if (mainLoop)
            {
                mainLoop();
            }
            KeyObject::Instance()->handleKeyEvent();
            draw();
            update();
        }
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 0;
    }

    void Core::applyPhysics(void)
    {
        for (auto &object : objects)
        {
            /* Only apply gravity if the object is not static */
            if (object->state() & State::STATIC)
            {
                continue;
            }
            else
            {
                /* Apply Gravity */
                object->move({0.0f, 3.0f});
                /* Check if the object is on the ground */
                if (object->data.position.y + (float)object->data.h == (float)SCREEN_HEIGHT)
                {
                    object->data.state |= State::ON_GROUND;
                }
                else
                {
                    object->data.state &= ~State::ON_GROUND;
                }
                for (auto &other : objects)
                {
                    if (&object == &other)
                    {
                        continue;
                    }
                }
            }
        }
    }

    void Core::clear(void)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void Core::draw(void)
    {
        for (auto const &object : objects)
        {
            object->draw(renderer);
        }
    }

    void Core::update(void)
    {
        // Update screen
        SDL_RenderPresent(renderer);
        // Delay so that we match framerate.
        SDL_Delay(timePerFrame * 1000);
    }

    void Core::pollForEvents(void)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
    }

    void Core::stop(void)
    {
        running = false;
    }

    unsigned int Core::getCurrentFrameCount(void) const noexcept
    {
        return frames;
    }

    unsigned int *Core::getCurrentFrameCountPtr(void) noexcept
    {
        return &frames;
    }

    void Core::set_title(const char *str)
    {
        window_title = str;
        SDL_SetWindowTitle(window, window_title);
    }

    vector<Object2D *> &Core::getObjects(void)
    {
        return objects;
    }

    Core *Core::Instance(void)
    {
        if (CoreInstance == nullptr)
        {
            lock_guard<mutex> lock(mutex);
            CoreInstance = new Core {};
        }
        return CoreInstance;
    }

}

namespace utils
{
    double screen_dpi(int sc_pix_w, int sc_pix_h, double sc_inch_w, double sc_inch_h)
    {
        double screen_dpi_x = sc_pix_w / sc_inch_w;
        double screen_dpi_y = sc_pix_h / sc_inch_h;
        return ((screen_dpi_x + screen_dpi_y) / 2.0);
    }

    SDL_Surface *create_sdl_surface_from_bitfield(MVector<MVector<int>> data)
    {
        unsigned int *pixels = AMALLOC_ARRAY(pixels, (data.size() * data.size()));
        for (int y = 0; y < data.size(); y++)
        {
            for (int x = 0; x < data.size(); x++)
            {
                int           brightness      = data[y][x];
                unsigned char grayscale_value = (brightness * 255) / 100;
                unsigned int  color           = (255 << 24) | (grayscale_value << 16) |
                                     (grayscale_value << 8) | grayscale_value;
                pixels[y * data.size() + x] = color;
            }
        }
        SDL_Surface *surface =
            SDL_CreateRGBSurfaceFrom(pixels, data.size(), data.size(), 32, data.size() * 4,
                                     0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        SDL_Surface *icon_surface_with_pixels =
            SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(surface);
        free(pixels);
        return icon_surface_with_pixels;
    }

    SDL_Texture *sdl_texture_exceptions_t::blank_texture = nullptr;
}

namespace sdl_structs
{
    /*-<< sdl_event_handler >>-*/
    sdl_event_handler *sdl_event_handler::instance = nullptr;

    sdl_event_handler *sdl_event_handler::Instance(void)
    {
        if (instance == nullptr)
        {
            instance = new sdl_event_handler_t();
        }
        return instance;
    }

    sdl_event_handler::~sdl_event_handler_t(void)
    {
        if (instance)
        {
            delete instance;
            instance = nullptr;
        }
    }

    void sdl_event_handler::handle_event(void)
    {
        while (SDL_PollEvent(&ev))
        {
            const unsigned int type = utils::event_to_index(ev.type);
            if (type == (unsigned int)-1 || map[type].empty())
            {
                continue;
            }
            for (auto const &func : map[type])
            {
                func(ev);
            }
        }
    }

    void sdl_element::draw(void)
    {
        if (text_data == nullptr)
        {
            logE("text_data == nullptr.");
            return;
        }
        if (text_data->texture == nullptr)
        {
            logE("texture is nullptr.");
            return;
        }
        SDL_Rect r = {rect.x + text_data->x, rect.y + text_data->y, text_data->w, text_data->h};
        if (SDL_RenderCopy(*renptr, text_data->texture, NULL, &r) != 0)
        {
            logE("SDL_RenderCopyFailed. SDL_ERROR: %s.", SDL_GetError());
        }
    }

    void sdl_element::add_text_data(int x, int y, SDL_Texture *texture)
    {
        if (texture == nullptr)
        {
            logE("texture == nullptr.");
            return;
        }
        sdl_element_text_t *data;
        LOG_AMALLOC(data);
        data->x       = x;
        data->y       = y;
        data->texture = texture;
        if (SDL_QueryTexture(texture, &data->format, &data->access, &data->w, &data->h) != 0)
        {
            logE("Failed to get query texture, SDL_ERROR: %s.", SDL_GetError());
            free(data);
        }
        text_data = data;
    }

    void sdl_element::render_rect(void)
    {
        SDL_SetRenderDrawColor(*renptr, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(*renptr, &rect);
    }

    /*-<< sdl_app >>-*/
    sdl_app *sdl_app ::_instance = nullptr;

    /*-<< Constructor >>-*/
    sdl_app::sdl_app_t(void)
    {
        init();
    }

    /*-<< Destructor >>-*/
    sdl_app ::~sdl_app_t(void)
    {
        if (_instance)
        {
            free(_instance);
        }
    }

    // Init the app.
    void sdl_app::init(void)
    {
        if (SDL_Init(SDL_INIT_VIDEO) == -1)
        {
            logE("Failed to init sdl, SDL_ERROR: %s.", SDL_GetError());
            exit(1);
        }
        if (TTF_Init() == -1)
        {
            logE("Failed to init ttf, SLD_ERROR: %s.", SDL_GetError());
            exit(1);
        }
        _window = SDL_CreateWindow((_title == nullptr ? "placeholder" : _title),
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height,
                                   SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_UTILITY);
        if (_window == nullptr)
        {
            logE("Failed to create window, SDL_ERROR: %s.", SDL_GetError());
            SDL_Quit();
            exit(1);
        }
        _renderer =
            SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (_renderer == nullptr)
        {
            logE("Failed to create renderer, SDL_ERROR: %s.", SDL_GetError());
            SDL_Quit();
            exit(1);
        }
        _running = true;
    }

    // Set current rendering color, should be set before drawing each element.
    void sdl_app::set_color(SDL_Color *color) const noexcept
    {
        SDL_SetRenderDrawColor(_renderer, color->r, color->g, color->b, color->a);
    }

    // Draw a rect.
    void sdl_app::draw_rect(SDL_Rect *rect) const noexcept
    {
        SDL_RenderFillRect(_renderer, rect);
    }

    void sdl_app::cleanup(void)
    {
        for (int i = 0; i < _elements.size(); i++)
        {
            free(_elements[i]->text_data);
            delete _elements[i];
        }
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        SDL_Quit();
    }

    // This is the only way to access the app.
    sdl_app *sdl_app::instance(void) noexcept
    {
        if (!_instance)
        {
            _instance = new (std::nothrow) sdl_app();
            if (_instance == nullptr)
            {
                logE("Mem alloc failed, 'new(std::nothrow)' returned a nullptr.");
                exit(1);
            }
        }
        return _instance;
    }

    // This is the rendering loop.
    int sdl_app::run(void) noexcept
    {
        while (_running)
        {
            frames++;
            SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
            SDL_RenderClear(_renderer);
            for (unsigned int i = 0; i < _elements.size(); i++)
            {
                _elements[i]->render_rect();
                _elements[i]->draw();
            }
            if (main_loop != nullptr)
            {
                main_loop();
            }
            SDL_EVENT_HANDLER->handle_event();
            SDL_KEY_HANDLER->handleKeyEvent();
            SDL_RenderPresent(_renderer);
            SDL_Delay(((double)1 / 120) * 1000);
        }
        cleanup();
        return 0;
    }

    // Set the main logic loop for the app.
    void sdl_app::set_main_loop(std::function<void()> loop)
    {
        if (loop != nullptr)
        {
            main_loop = loop;
        }
    }

    // Add a element to the app.
    sdl_element *sdl_app::new_element(sdl_element element) noexcept
    {
        sdl_element *new_element = new sdl_element {element};
        new_element->renptr      = &_renderer;
        _elements.push_back(new_element);
        return new_element;
    }

    // Tell the app to stop running.
    void sdl_app::quit(void) noexcept
    {
        _running = false;
    }

    // Retrieve a font file at 'size' and save it for later use with 'name'.
    TTF_Font *sdl_app::retrieve_new_font(const char *file, int size, const char *name)
    {
        TTF_Font *font = TTF_OpenFont(file, size);
        if (font == nullptr)
        {
            logE("Failed to open font file '%s'.", file);
        }
        return font;
    }

    SDL_Texture *sdl_app::make_text_texture(TTF_Font *font, const char *text, SDL_Color color)
    {
        /* If the 'text' string is empty we do not return a nullptr,
         * instead we return an texture exception.  A blank 1x1 texture. */
        if (text[0] == '\0')
        {
            if (sdl_texture_exceptions::blank_texture == nullptr)
            {
                sdl_texture_exceptions::blank_texture = SDL_CreateTexture(
                    _renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
                if (sdl_texture_exceptions::blank_texture == nullptr)
                {
                    logE("Failed to create 'sdl_texture_exceptions::blank_texture'.");
                }
            }
            return sdl_texture_exceptions::blank_texture;
        }
        SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, color);
        if (surface == nullptr)
        {
            logE("Failed to make surface.");
            return nullptr;
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
        if (texture == nullptr)
        {
            logE("Failed to make texture.");
            return nullptr;
        }
        SDL_FreeSurface(surface);
        return texture;
    }

    void sdl_app::render_text(int x, int y, SDL_Texture *texture)
    {
        if (texture == nullptr)
        {
            logE("texture is nullptr.");
            return;
        }
        if (texture == sdl_texture_exceptions::blank_texture)
        {
            return;
        }
        int w;
        int h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderCopy(_renderer, texture, NULL, &rect);
    }

    void sdl_app::move_element(unsigned int id, int x, int y)
    {
        sdl_element_t *element = element_from_id(id);
        if (element)
        {
            element->rect.x = x;
            element->rect.y = y;
        }
    }

    int sdl_app::width(void) const noexcept
    {
        return _width;
    }

    int sdl_app::height(void) const noexcept
    {
        return _height;
    }

    void sdl_app::set_width(int w) noexcept
    {
        _width = w;
    }

    void sdl_app::set_height(int h) noexcept
    {
        _height = h;
    }

    sdl_element *sdl_app::element_from_id(unsigned int id)
    {
        for (int i = 0; i < _elements.size(); i++)
        {
            if (_elements[i]->id == id)
            {
                return _elements[i];
            }
        }
        return nullptr;
    }

    void sdl_app ::add_text_to_window(unsigned int id, TTF_Font *font, const string &text,
                                      SDL_Color color, int x, int y)
    {
        if (font == nullptr)
        {
            return;
        }
        sdl_element_t *window = element_from_id(id);
        if (window == nullptr)
        {
            return;
        }
        SDL_Texture *texture = make_text_texture(font, text.c_str(), color);
        if (texture == nullptr)
        {
            return;
        }
    }
}

#endif
