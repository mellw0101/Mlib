#pragma once

#include <cstdlib>
#include <functional>

#include <unordered_map>
#include <vector>

#include <SDL3/SDL.h>

using key_handler_map = std::unordered_map<unsigned char, std::vector<std::function<void()>>>;
/* This Class Represents The Keyboard Object, it Is Used To Handle Keyboard
 * Events It Is The Interface Between The Keyboard And the Engine Note This
 * Class Is A Singleton Class, And Can Only Be Accessed Through The
 * 'Instance' Function.  This Class Is Used To Add Actions For Keys It Runs
 * Every Frame And Executes All Lambda Functions Added Using The
 * 'addActionForKey' Function */
class key_handler {
private:
    key_handler_map     _map;
    static key_handler *_instance;

    static void _destroy(void) noexcept;

public:
    static key_handler *Instance(void) noexcept;

    /* This function adds an action for a key, using perfect forwarding. */
    template <typename F, typename... Args>
    void
    addActionForKey(unsigned char key, F &&func, Args &&...args)
    {
        auto boundFunc = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
        _map[key].emplace_back([boundFunc]() {
            boundFunc();
        });
    }

    void handleKeyEvent(void);
};
