/// @file Sys.h
#pragma once

#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "def.h"

using namespace std;

namespace Mlib::Sys {
    s8 run_binary(const string &binary_path, const vector<string> &args, const vector<string> &env_vars = {});

    class Prompt
    {
    public:
        Prompt(const string &prompt);
        operator string() const;

    private:
        stringstream ss;
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
        getInstance(T val)
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
} // namespace Mlib::Sys
