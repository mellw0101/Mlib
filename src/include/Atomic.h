/// @file Atomic.h
#pragma once

#include <functional>
#include <iostream>
#include <mutex>
#include <type_traits>
#include <vector>

#include "../include/def.h"

namespace Mlib::Atomic {
    template <typename T>
    class Atomic
    {
    public:
        Atomic(T value)
            : value(value)
        {}

        T
        get() const
        {
            return value;
        }

        void
        set(T val)
        {
            value = val;
        }

        T
        operator++()
        {
            return __sync_add_and_fetch(&value, 1);
        }

        T
        operator--()
        {
            return __sync_sub_and_fetch(&value, 1);
        }

        T
        operator++(int)
        {
            return __sync_fetch_and_add(&value, 1);
        }

        T
        operator--(int)
        {
            return __sync_fetch_and_sub(&value, 1);
        }

        T
        operator+=(T val)
        {
            return __sync_add_and_fetch(&value, val);
        }

        T
        operator-=(T val)
        {
            return __sync_sub_and_fetch(&value, val);
        }

    private:
        T value;
    };

    template <typename T>
    class SignalSingleton
    {
        static_assert(is_arithmetic<T>::value, "Template parameter must be a numeric type");

    public:
        using Signal = function<void(T)>;

        // Delete copy constructor and assignment operator
        SignalSingleton(const SignalSingleton&)            = delete;
        SignalSingleton& operator=(const SignalSingleton&) = delete;

        // Static method to get the singleton instance
        static SignalSingleton&
        getInstance()
        {
            static SignalSingleton instance;
            return instance;
        }

        // Method to connect a signal (callback function)
        void
        connect(Signal signal)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            signals_.push_back(signal);
        }

        // Method to emit signals
        void
        emit(T value)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            for (auto& signal : signals_)
            {
                signal(value);
            }
        }

    private:
        // Private constructor to prevent direct instantiation
        SignalSingleton() = default;

        std::vector<Signal> signals_;
        std::mutex          mutex_;
    };

} // namespace Mlib::Atomic
