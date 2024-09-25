#include <SDL3/SDL_keyboard.h>
#include <cstdlib>
#include <new>

#include "SDL/key_handler.hpp"

key_handler *key_handler::_instance = nullptr;

void
key_handler::_destroy(void) noexcept
{
    delete _instance;
}

key_handler *
key_handler::Instance(void) noexcept
{
    if (!_instance)
    {
        _instance = new (std::nothrow) key_handler();
        if (!_instance)
        {
            exit(1);
        }
        atexit(_destroy);
    }
    return _instance;
}

void
key_handler::handleKeyEvent(void)
{
    const bool *state = SDL_GetKeyboardState(nullptr);
    for (auto const &[key, funcs] : _map)
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
