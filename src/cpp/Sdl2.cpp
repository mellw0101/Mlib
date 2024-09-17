/// @file Sdl2.cpp
/// @brief Contains the implementation of the Sdl2 namespace.
/// @note This file defines the implementation of the Sdl2 namespace.
#include "../include/Sdl2.h"
#include "../include/Debug.h"

#if defined(__i386__) || defined(__x86_64__)

#    include <SDL2/SDL_ttf.h>
#    include "../include/Loops.h"

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

    void convert_sdl_rect_to_floats(const SDL_Rect *rect, float *floats)
    {
        floats[0] = static_cast<float>(rect->x);
        floats[1] = static_cast<float>(rect->y);
        floats[2] = static_cast<float>(rect->w);
        floats[3] = static_cast<float>(rect->h);
    }

    void sdl_simd_update_rect(SDL_FRect *frect, const SDL_FRect *end_frect, const float *step_vals)
    {
        // Load the current rectangle and the end rectangle into SIMD registers
        __m128 rect_vals = _mm_loadu_ps(
            reinterpret_cast<const float *>(frect)); // frect.x, frect.y, frect.w, frect.h
        __m128 end_vals       = _mm_loadu_ps(reinterpret_cast<const float *>(
            end_frect)); // end_frect.x, end_frect.y, end_frect.w, end_frect.h
        __m128 step_vals_simd = _mm_loadu_ps(step_vals); // Load step values for x, y, w, h

        // Step comparisons: step < 0.0f
        __m128 step_lt_zero = _mm_cmplt_ps(step_vals_simd, _mm_set1_ps(0.0f));

        // Condition when step < 0.0 and rect < end
        __m128 rect_lt_end    = _mm_cmplt_ps(rect_vals, end_vals);
        __m128 use_end_for_lt = _mm_and_ps(step_lt_zero, rect_lt_end);

        // Step comparisons: step > 0.0f
        __m128 step_gt_zero = _mm_cmpgt_ps(step_vals_simd, _mm_set1_ps(0.0f));

        // Condition when step > 0.0 and rect > end
        __m128 rect_gt_end    = _mm_cmpgt_ps(rect_vals, end_vals);
        __m128 use_end_for_gt = _mm_and_ps(step_gt_zero, rect_gt_end);

        // Use the appropriate values based on the conditions
        __m128 use_end      = _mm_or_ps(use_end_for_lt, use_end_for_gt);
        __m128 updated_rect = _mm_blendv_ps(rect_vals, end_vals, use_end);

        // Store the updated values back into the rectangle
        _mm_storeu_ps(reinterpret_cast<float *>(frect), updated_rect);
    }

    void sdl_simd_update_rect_avx(SDL_FRect *frect, const SDL_FRect *end_frect,
                                  const float *step_vals)
    {
        // Load the current rectangle and the end rectangle into AVX registers
        __m256 rect_vals = _mm256_loadu_ps(
            reinterpret_cast<const float *>(frect)); // frect.x, frect.y, frect.w, frect.h
        __m256 end_vals      = _mm256_loadu_ps(reinterpret_cast<const float *>(
            end_frect)); // end_frect.x, end_frect.y, end_frect.w, end_frect.h
        __m256 step_vals_avx = _mm256_loadu_ps(step_vals); // Load step values for x, y, w, h

        // Step comparisons: step < 0.0f
        __m256 step_lt_zero = _mm256_cmp_ps(step_vals_avx, _mm256_set1_ps(0.0f), _CMP_LT_OQ);

        // Condition when step < 0.0 and rect < end
        __m256 rect_lt_end    = _mm256_cmp_ps(rect_vals, end_vals, _CMP_LT_OQ);
        __m256 use_end_for_lt = _mm256_and_ps(step_lt_zero, rect_lt_end);

        // Step comparisons: step > 0.0f
        __m256 step_gt_zero = _mm256_cmp_ps(step_vals_avx, _mm256_set1_ps(0.0f), _CMP_GT_OQ);

        // Condition when step > 0.0 and rect > end
        __m256 rect_gt_end    = _mm256_cmp_ps(rect_vals, end_vals, _CMP_GT_OQ);
        __m256 use_end_for_gt = _mm256_and_ps(step_gt_zero, rect_gt_end);

        // Use the appropriate values based on the conditions
        __m256 use_end      = _mm256_or_ps(use_end_for_lt, use_end_for_gt);
        __m256 updated_rect = _mm256_blendv_ps(rect_vals, end_vals, use_end);

        // Store the updated values back into the rectangle
        _mm256_storeu_ps(reinterpret_cast<float *>(frect), updated_rect);
    }
}

namespace sdl
{
    int set_render_draw_color(SDL_Renderer *ren, unsigned char r, unsigned char g, unsigned char b,
                              unsigned char a) noexcept
    {
        int result = SDL_SetRenderDrawColor(ren, r, g, b, a);
        if (result == 0)
        {
            return 0;
        }
        logE("SDL_SetRenderColor failed, code %i, SDL_ERROR: ('%s').", result, SDL_GetError());
        return result;
    }
}

namespace sdl_structs
{
    /*-<< sdl_event_handler >>-*/
    sdl_event_handler *sdl_event_handler::instance = nullptr;

    sdl_event_handler *sdl_event_handler::Instance(void) noexcept
    {
        if (instance == nullptr)
        {
            instance = new (nothrow) sdl_event_handler();
            if (instance == nullptr)
            {
                logE("Mem alloc failed, 'new(nothrow)' return`ed 'nullptr'.");
                SDL_APP->exit_clean(1);
            }
            atexit(destroy);
        }
        return instance;
    }

    void sdl_event_handler::destroy(void) noexcept
    {
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        }
    }

    void sdl_event_handler::handle_event(void) noexcept
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

    /*-<< sdl_element_text >>-*/
    void sdl_element_text::query_text_texture(int x, int y, SDL_Texture *t) noexcept
    {
        (x != -1) ? this->x = x : 0;
        (y != -1) ? this->y = y : 0;
        (t != nullptr) ? texture = t : 0;
        if (SDL_QueryTexture(texture, &format, &access, &w, &h) != 0)
        {
            logE("Failed to query texture, SDL_ERROR: %s.", SDL_GetError());
        }
    }

    /*-<< sdl_element >>-*/
    void sdl_element::draw_borders(void) noexcept
    {
        if (border_size <= 0 || border_size >= rect.w || border_size >= rect.h)
        {
            return;
        }
        SDL_SetRenderDrawColor(
            *renptr, border_color.r, border_color.g, border_color.b, border_color.b);
        SDL_Rect r;
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

    void sdl_element::draw(void) noexcept
    {
        sdl::set_render_draw_color(*renptr, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(*renptr, &rect);
        draw_borders();
        if (text_data == nullptr)
        {
            return;
        }
        if (text_data->texture == nullptr)
        {
            logE("texture is nullptr.");
            return;
        }
        if (text_data->texture == sdl_texture_exceptions::blank_texture)
        {
            return;
        }
        SDL_Rect r = {rect.x + text_data->x, rect.y + text_data->y, text_data->w, text_data->h};
        if (SDL_RenderCopy(*renptr, text_data->texture, NULL, &r) != 0)
        {
            logE("SDL_RenderCopyFailed. SDL_ERROR: %s.", SDL_GetError());
        }
    }

    void sdl_element::add_text_data(int x, int y, SDL_Texture *texture) noexcept
    {
        if (texture == nullptr)
        {
            logE("texture == nullptr.");
            return;
        }
        if (text_data == nullptr)
        {
            text_data = SDL_APP->mem_pool.alloc<sdl_element_text>();
            if (text_data == nullptr)
            {
                SDL_APP->exit_clean(1);
            }
        }
        text_data->query_text_texture(x, y, texture);
    }

    void sdl_element::animate(int end_x, int end_y, int end_w, int end_h, int duration_ms) noexcept
    {
        if (in_animation == false)
        {
            logI("duration_ms: %i", duration_ms);
            double steps = duration_ms / FRAME_DELAY(SDL_APP->get_framerate());
            logI("steps: %lf", steps);
            float step_x = (end_x - rect.x) / steps;
            float step_y = (end_y - rect.y) / steps;
            logI("step_y: %lf", step_y);
            float                      step_w = (end_w - rect.w) / steps;
            float                      step_h = (end_h - rect.h) / steps;
            sdl_element_animation_data animation_data {
                steps, end_x, end_y, end_w, end_h, step_x, step_y, step_w, step_h,
            };
            frect.x = rect.x;
            frect.y = rect.y;
            frect.w = rect.w;
            frect.h = rect.h;
            SDL_APP->animate_element(this, animation_data);
        }
    }

    void sdl_element::set_borders(int size, SDL_Color color) noexcept
    {
        border_size  = size;
        border_color = color;
    }

    /* Constructor. */
    sdl_element::sdl_element_t(SDL_Color c, SDL_Rect r) noexcept
    {
        color        = c;
        rect         = r;
        frect        = {0.00, 0.00, 0.00, 0.00};
        id           = 0;
        in_animation = false;
        text_data    = nullptr;
        renptr       = nullptr;
        border_size  = 0;
    }

    /*-<< sdl_button_element >>-*/
    void sdl_button_element::add_callback(void) noexcept
    {
        SDL_EVENT_HANDLER->add_action_for_event(SDL_MOUSEBUTTONDOWN, [this](SDL_Event ev) {
            SDL_MouseButtonEvent e = ev.button;
            if ((e.x >= rect.x && e.x <= rect.x + rect.w) &&
                (e.y >= rect.y && e.y <= rect.y + rect.h))
            {
                if (_action != nullptr)
                {
                    _action(e);
                }
            }
        });
    }

    /*-<< sdl_app >>-*/
    sdl_app *sdl_app::_instance = nullptr;

    /*-<< Constructor >>-*/
    sdl_app::sdl_app_t(void) noexcept
        : mem_pool(100 * MB)
    {
        if (SDL_Init(SDL_INIT_VIDEO) == -1)
        {
            logE("Failed to init sdl, SDL_ERROR: %s.", SDL_GetError());
            exit(1);
        }
        if (TTF_Init() == -1)
        {
            logE("Failed to init ttf, SLD_ERROR: %s.", SDL_GetError());
            SDL_Quit();
            exit(1);
        }
        _win = SDL_CreateWindow((_title == nullptr ? "placeholder" : _title),
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (_win == nullptr)
        {
            logE("Failed to create window, SDL_ERROR: %s.", SDL_GetError());
            TTF_Quit();
            SDL_Quit();
            exit(1);
        }
        _ren = SDL_CreateRenderer(_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (_ren == nullptr)
        {
            logE("Failed to create renderer, SDL_ERROR: %s.", SDL_GetError());
            TTF_Quit();
            SDL_Quit();
            exit(1);
        }
        _running = true;
        SDL_EVENT_HANDLER->add_action_for_event(SDL_WINDOWEVENT, [this](SDL_Event ev) {
            SDL_WindowEvent e = ev.window;
            switch (e.event)
            {
                case SDL_WINDOWEVENT_RESIZED :
                {
                    _width  = e.data1;
                    _height = e.data2;
                    break;
                }
            }
        });
        SDL_EVENT_HANDLER->add_action_for_event(SDL_QUIT, [this](SDL_Event ev) {
            quit();
        });
        a = (double *)mem_pool.alloc(32, 32);
        b = (double *)mem_pool.alloc(32, 32);
        r = (double *)mem_pool.alloc(32, 32);
    }

    /*-<< Destructor >>-*/
    void sdl_app::destroy(void) noexcept
    {
        if (_instance != nullptr)
        {
            delete _instance;
            _instance = nullptr;
        }
    }

    /* Dealloc all obj saved during runtime, this way all
     * heap mem is handled centraly and automaticly. */
    void sdl_app::cleanup(void) noexcept
    {
        if (sdl_texture_exceptions::blank_texture != nullptr)
        {
            SDL_DestroyTexture(sdl_texture_exceptions::blank_texture);
        }
        for (int i = 0; i < _alloced_textures.size(); i++)
        {
            SDL_DestroyTexture(_alloced_textures[i]);
            /* Sanity check so that i know that no textures got destroyd before this. */
            if (strcmp(SDL_GetError(), "Invalid texture") == 0)
            {
                logE("SDL_DestroyTexture recieved a invalid alloced texture.");
            }
        }
        for (int i = 0; i < _alloced_fonts.size(); i++)
        {
            TTF_CloseFont(_alloced_fonts[i]);
        }
        SDL_DestroyRenderer(_ren);
        SDL_DestroyWindow(_win);
        TTF_Quit();
        SDL_Quit();
    }

    void sdl_app::perform_amimation(void) noexcept
    {
        for (unsigned int i = 0; i < _anim_data.size(); i++)
        {
            auto &[steps, end_x, end_y, end_w, end_h, step_x, step_y, step_w, step_h] =
                _anim_data[i].second;
            if (steps <= 0.00)
            {
                _anim_data[i].first->in_animation = false;
                _anim_data[i].first->rect.x       = end_x;
                _anim_data[i].first->rect.y       = end_y;
                _anim_data[i].first->rect.w       = end_w;
                _anim_data[i].first->rect.h       = end_h;
                _anim_data.erase_at(i);
                i -= 1;
            }
            else
            {
                PROFILE_FUNCTION;
                steps -= 1.00;
                float     step_vals[4] = {static_cast<float>(step_x), static_cast<float>(step_y),
                                          static_cast<float>(step_w), static_cast<float>(step_h)};
                SDL_FRect end_rect     = {static_cast<float>(end_x), static_cast<float>(end_y),
                                          static_cast<float>(end_w), static_cast<float>(end_h)};
                float     rect_floats[4];
                utils::convert_sdl_rect_to_floats(&_anim_data[i].first->rect, rect_floats);
                utils::sdl_simd_update_rect(&_anim_data[i].first->frect, &end_rect, step_vals);
                /* a[0] = _anim_data[i].first->frect.x;
                a[1] = _anim_data[i].first->frect.y;
                a[2] = _anim_data[i].first->frect.w;
                a[3] = _anim_data[i].first->frect.h;
                b[0] = step_x;
                b[1] = step_y;
                b[2] = step_w;
                b[3] = step_h;
                AVX_SIMD_add_4_double(&a, &b);
                _anim_data[i].first->frect.x = a[0];
                _anim_data[i].first->frect.y = a[1];
                _anim_data[i].first->frect.w = a[2];
                _anim_data[i].first->frect.h = a[3];
                _anim_data[i].first->rect.x  = _anim_data[i].first->frect.x;
                if ((step_x < 0.00 && _anim_data[i].first->rect.x < end_x) ||
                    (step_x > 0.00 && _anim_data[i].first->rect.x > end_x))
                {
                    _anim_data[i].first->rect.x = end_x;
                }
                _anim_data[i].first->rect.y = _anim_data[i].first->frect.y;
                if ((step_y < 0.00 && _anim_data[i].first->rect.y < end_y) ||
                    (step_y > 0.00 && _anim_data[i].first->rect.y > end_y))
                {
                    _anim_data[i].first->rect.y = end_y;
                }
                _anim_data[i].first->rect.w = _anim_data[i].first->frect.w;
                if ((step_w < 0.00 && _anim_data[i].first->rect.w < end_w) ||
                    (step_w > 0.00 && _anim_data[i].first->rect.w > end_w))
                {
                    _anim_data[i].first->rect.w = end_w;
                }
                _anim_data[i].first->rect.h = _anim_data[i].first->frect.h;
                if ((step_h < 0.00 && _anim_data[i].first->rect.h < end_h) ||
                    (step_h > 0.00 && _anim_data[i].first->rect.h > end_h))
                {
                    _anim_data[i].first->rect.h = end_h;
                } */
            }
        }
    }

    void sdl_app::frame_start(void) noexcept
    {
        frame_start_tick = SDL_GetTicks();
    }

    void sdl_app::frame_end(void) noexcept
    {
        const unsigned int frame_time = SDL_GetTicks() - frame_start_tick;
        if (frame_time < FRAME_DELAY(_framerate))
        {
            SDL_Delay(FRAME_DELAY(_framerate) - frame_time);
        }
    }

    /* This is the only way to access the app. */
    sdl_app *sdl_app::instance(void) noexcept
    {
        if (_instance == nullptr)
        {
            _instance = new (nothrow) sdl_app();
            if (_instance == nullptr)
            {
                logE("Mem alloc failed, 'new(std::nothrow)' returned a nullptr.");
                exit(1);
            }
            atexit(destroy);
        }
        return _instance;
    }

    /* This is the rendering loop. */
    int sdl_app::run(void) noexcept
    {
        set_position(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        while (_running)
        {
            frame_start();
            SDL_SetRenderDrawColor(_ren, 0, 0, 0, 255);
            SDL_RenderClear(_ren);
            perform_amimation();
            for (unsigned int i = 0; i < _elements.size(); i++)
            {
                _elements[i]->draw();
            }
            if (_main_loop != nullptr)
            {
                _main_loop();
            }
            SDL_EVENT_HANDLER->handle_event();
            SDL_KEY_HANDLER->handleKeyEvent();
            SDL_RenderPresent(_ren);
            frame_end();
        }
        cleanup();
        return 0;
    }

    /* Set the main logic loop for the app. */
    void sdl_app::set_main_loop(function<void()> loop) noexcept
    {
        if (loop != nullptr)
        {
            _main_loop = loop;
        }
    }

    /* Add a element to the app. */
    sdl_element *sdl_app::new_element(sdl_element element) noexcept
    {
        sdl_element *new_element = mem_pool.alloc<sdl_element>();
        if (new_element == nullptr)
        {
            logE("Mem alloc failed, 'new(nothrow)' return`ed 'nullptr'.");
            exit_clean(1);
        }
        new_element->color  = element.color;
        new_element->rect   = element.rect;
        new_element->renptr = &_ren;
        _elements.push_back(new_element);
        logI("mem_pool used: %lu.", mem_pool.used_memory());
        return new_element;
    }

    /* Add a element to the app. */
    sdl_button_element *sdl_app::new_button_element(sdl_element element) noexcept
    {
        sdl_button_element *new_element = mem_pool.alloc<sdl_button_element>();
        if (new_element == nullptr)
        {
            logE("Mem alloc failed, 'new(nothrow)' return`ed 'nullptr'.");
            exit_clean(1);
        }
        new_element->color  = element.color;
        new_element->rect   = element.rect;
        new_element->renptr = &_ren;
        _elements.push_back(new_element);
        logI("mem_pool used: %lu.", mem_pool.used_memory());
        return new_element;
    }

    /* Tell the app to stop running. */
    void sdl_app::quit(void) noexcept
    {
        _running = false;
    }

    /* Retrieve a font file at 'size', do NOT free the font for any reason the alloced
     * memory is fully managed by 'sdl_app' internaly. */
    TTF_Font *sdl_app::retrieve_new_font(const int size, const char *file) noexcept
    {
        TTF_Font *font = TTF_OpenFont(file, size);
        if (font == nullptr)
        {
            logE("Failed to open font file '%s'.", file);
            return nullptr;
        }
        _alloced_fonts.push_back(font);
        return font;
    }

    SDL_Texture *sdl_app::make_text_texture(TTF_Font *font, const char *text, SDL_Color fg,
                                            SDL_Color bg) noexcept
    {
        /* If the 'text' string is empty we do not return a nullptr,
         * instead we return an texture exception.  A blank 1x1 texture. */
        if (text[0] == '\0')
        {
            if (sdl_texture_exceptions::blank_texture == nullptr)
            {
                sdl_texture_exceptions::blank_texture = SDL_CreateTexture(
                    _ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
                if (sdl_texture_exceptions::blank_texture == nullptr)
                {
                    logE("Failed to create 'sdl_texture_exceptions::blank_texture'.");
                }
            }
            return sdl_texture_exceptions::blank_texture;
        }
        SDL_Surface *surface = TTF_RenderUTF8_Shaded(font, text, fg, bg);
        if (surface == nullptr)
        {
            logE("Failed to make surface.");
            return nullptr;
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(_ren, surface);
        if (texture == nullptr)
        {
            logE("Failed to make texture.");
            return nullptr;
        }
        SDL_FreeSurface(surface);
        if (_alloced_textures.size() > 100)
        {
            for (int i = 0; i < 20; i++)
            {
                SDL_DestroyTexture(_alloced_textures[i]);
            }
            _alloced_textures.reorder_from(20);
        }
        _alloced_textures.push_back(texture);
        return texture;
    }

    SDL_Texture *sdl_app::make_destroy_text_texture(SDL_Texture *old_texture, TTF_Font *font,
                                                    const char *text, SDL_Color fg,
                                                    SDL_Color bg) noexcept
    {
        if (old_texture != nullptr)
        {
            for (unsigned int i = 0; i < _alloced_textures.size(); i++)
            {
                if (_alloced_textures[i] == old_texture)
                {
                    SDL_DestroyTexture(_alloced_textures[i]);
                    _alloced_textures.erase_at(i);
                    break;
                }
            }
        }
        return make_text_texture(font, text, fg, bg);
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
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderCopy(_ren, texture, nullptr, &rect);
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

    /* Window config methods. */
    void sdl_app::set_size(const int w, const int h) noexcept
    {
        SDL_SetWindowSize(_win, w, h);
        _width  = w;
        _height = h;
    }

    void sdl_app::set_position(const int x, const int y) noexcept
    {
        SDL_SetWindowPosition(_win, x, y);
    }

    void sdl_app::set_title(const char *str) noexcept
    {
        SDL_SetWindowTitle(_win, str);
    }

    void sdl_app::set_min_size(const int w, const int h) noexcept
    {
        SDL_SetWindowMinimumSize(_win, w, h);
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

    void sdl_app::exit_clean(const int status) noexcept
    {
        cleanup();
        exit(status);
    }

    int sdl_app::set_framerate(const int new_framerate) noexcept
    {
        _framerate = std::move(new_framerate);
        return _framerate;
    }

    int sdl_app::get_framerate(void) const noexcept
    {
        return _framerate;
    }

    void sdl_app::animate_element(sdl_element *element, sdl_element_animation_data data) noexcept
    {
        if (element->in_animation == false)
        {
            element->in_animation = true;
            _anim_data.push_back({element, data});
        }
    }
}

#endif
