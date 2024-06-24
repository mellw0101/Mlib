/// @file Sys.h
#pragma once

#include <cstring>
#include <iostream>
#include <spawn.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "def.h"

namespace Mlib::Sys
{
    s8 run_binary(const std::string &binary_path, const std::vector<std::string> &args,
                  const std::vector<std::string> &env_vars = {});

    class Prompt
    {
    public:
        Prompt(const std::string &prompt);
        operator std::string() const;

    private:
        std::stringstream ss;
    };

    template <typename T>
    class Singleton
    {
    private:
        static Singleton *instance;
        T                 value;

        Singleton(T val)
            : value(val)
        {}

    public:
        static Singleton *
        Instance(T val)
        {
            if (instance == nullptr)
            {
                instance = new Singleton(val);
            }
            return instance;
        }

        static void
        deleteInstance()
        {
            delete instance;
            instance = nullptr;
        }

        T
        getValue() const
        {
            return value;
        }

        void
        setValue(T val)
        {
            value = val;
        }

        ~Singleton()
        {
            std::cout << "Singleton destructor called" << std::endl;
        }
    };

    s32 launch_child_process(const s8 *command);

} // namespace Mlib::Sys
