/// @file Sdl2.cpp
/// @brief Contains the implementation of the Sdl2 namespace.
/// @note This file defines the implementation of the Sdl2 namespace.
#include "../include/Sdl2.h"
#if defined(__i386__) || defined(__x86_64__)

namespace Mlib::Sdl2
{
    inline int SCREEN_WIDTH;
    inline int SCREEN_HEIGHT;

    KeyObject *KeyObject::KeyObjectInstance = nullptr;
    Core      *Core::CoreInstance           = nullptr;

    /// @struct @c Object2D Function Definitions

    void
    Object2D ::init(ObjectData2D const &data)
    {
        this->data = data;
    }

    void
    Object2D ::move(Vec2D vel)
    {
        vel.x = (data.position.x + data.w + vel.x >= SCREEN_WIDTH) ? SCREEN_WIDTH - data.position.x - data.w : vel.x;
        vel.y = (data.position.y + data.h + vel.y >= SCREEN_HEIGHT) ? SCREEN_HEIGHT - data.position.y - data.h : vel.y;
        vel.x = (data.position.x + vel.x < 0) ? -data.position.x : vel.x;
        vel.y = (data.position.y + vel.y < 0) ? -data.position.y : vel.y;

        data.position += vel;
    }

    void
    Object2D ::draw(SDL_Renderer *renderer) const
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect currentRect = rect();
        SDL_RenderFillRect(renderer, &currentRect);
    }

    bool
    Object2D ::isStatic(void) const
    {
        return data.state & State::STATIC;
    }

    SDL_Rect
    Object2D ::rect(void) const
    {
        SDL_Rect rect = {static_cast<int>(data.position.x), static_cast<int>(data.position.y), data.w, data.h};
        return rect;
    }

    SDL_FRect
    Object2D ::frect(void) const
    {
        return {static_cast<f32>(data.position.x), static_cast<f32>(data.position.y), static_cast<f32>(data.w),
                static_cast<f32>(data.h)};
    }

    unsigned int
    Object2D ::state(unsigned int stateToCheck) const noexcept
    {
        if (stateToCheck)
        {
            return data.state & stateToCheck;
        }
        return data.state;
    }

    void
    Object2D ::updateVelocity(void)
    {
        /* Calculate velocity change due to acceleration */
        Vec2D const velocityChange = GravityVec * timePerFrame;
        /* Update velocity */
        data.velocity += velocityChange;
        /* Update position */
        data.position += data.velocity;
    }

    void
    Object2D ::onStaticObjCollision(Object2D const &obj, Vec2D velVecToApply)
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

    /// @class @c Core Function Definitions

    void
    Core ::createObject(const Object2D &object)
    {
        Object2D *nObj = new Object2D {object};
        objects.emplace_back(nObj);
    }

    int
    Core ::init(void)
    {
        initSDL();
        createWindow();
        createRenderer();
        // setupKeys();
        return 0;
    }

    int
    Core ::initSDL(void)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            running = false;
            return -1;
        }
        return 0;
    }

    int
    Core ::createWindow(void)
    {
        window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window)
        {
            SDL_Quit();
            return -1;
        }
        return 0;
    }

    int
    Core ::createRenderer(void)
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            SDL_DestroyWindow(window);
            SDL_Quit();
            return -1;
        }
        return 0;
    }

    void
    Core ::setupKeys(void)
    {
        KeyObject::Instance()->addActionForKey(
            SDL_SCANCODE_W,
            [&]() -> void
            {
                for (auto &object : objects)
                {
                    ((object->state() & State::STATIC) == false) ? object->move({0.0, -object->data.speed}) : void();
                }
            });

        KeyObject::Instance()->addActionForKey(
            SDL_SCANCODE_S,
            [&]() -> void
            {
                for (auto &object : objects)
                {
                    ((object->state() & State::STATIC) == false) ? object->move({0.0, object->data.speed}) : void();
                }
            });
        KeyObject::Instance()->addActionForKey(
            SDL_SCANCODE_A,
            [&]() -> void
            {
                for (auto &object : objects)
                {
                    ((object->state() & State::STATIC) == false) ? object->move({-object->data.speed, 0.0}) : void();
                }
            });
        KeyObject::Instance()->addActionForKey(
            SDL_SCANCODE_D,
            [&]() -> void
            {
                for (auto &object : objects)
                {
                    !(object->state() & State::STATIC) ? object->move({object->data.speed, 0.0}) : void();
                }
            });
        KeyObject::Instance()->addActionForKey(SDL_SCANCODE_ESCAPE,
                                               [&]() -> void
                                               {
                                                   running = false;
                                               });
        KeyObject::Instance()->addActionForKey(SDL_SCANCODE_SPACE,
                                               [&]() -> void
                                               {
                                                   for (auto &object : objects)
                                                   {
                                                       // If the object is static, skip
                                                       if (object->state() & State::STATIC)
                                                       {
                                                           continue;
                                                       }

                                                       // If the object is a player, jump
                                                       if (object->state() & State::IS_PLAYER)
                                                       {
                                                           object->move({0, -10});
                                                       }
                                                   }
                                               });
    }

    int
    Core ::run(void)
    {
        init();
        while (running)
        {
            frames++;
            pollForEvents();
            // applyPhysics();
            if (mainLoop)
            {
                mainLoop();
            }
            logic();
            clear();
            draw();
            update();
        }
        cleanup();
        return 0;
    }

    void
    Core ::cleanup(void)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void
    Core ::logic(void)
    {
        KeyObject::Instance()->handleKeyEvent();
    }

    void
    Core ::applyPhysics(void)
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
                if (object->data.position.y + (f32)object->data.h == (f32)SCREEN_HEIGHT)
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

    void
    Core ::clear(void)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void
    Core ::draw()
    {
        for (auto const &object : objects)
        {
            object->draw(renderer);
        }
    }

    void
    Core ::update()
    {
        SDL_RenderPresent(renderer);    // Update screen
        SDL_Delay(timePerFrame * 1000); // Approximately 60 frames per second
    }

    void
    Core ::pollForEvents()
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
    }

    void
    Core ::stop()
    {
        running = false;
    }

    u32
    Core ::getCurrentFrameCount() const noexcept
    {
        return frames;
    }

    u32 *
    Core ::getCurrentFrameCountPtr() noexcept
    {
        return &frames;
    }

    void
    Core ::setTitle(string const &title)
    {
        window_title = title;
        SDL_SetWindowTitle(window, window_title.c_str());
    }

    vector<Object2D *> &
    Core ::getObjects()
    {
        return objects;
    }

    Core *
    Core ::Instance()
    {
        if (CoreInstance == nullptr)
        {
            lock_guard<mutex> lock(mutex);
            CoreInstance = new Core {};
        }
        return CoreInstance;
    }
} // namespace Mlib::Sdl2

#endif
