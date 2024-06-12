#include "../include/Assert.h"

namespace Mlib::Assert {
    void
    isTrue(bool condition, const std::string& message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }

    void
    isFalse(bool condition, const std::string& message)
    {
        if (condition)
        {
            throw std::runtime_error(message);
        }
    }

    void
    isNotNull(void* value, const std::string& message)
    {
        if (value == nullptr)
        {
            throw std::runtime_error(message);
        }
    }

    void
    isNull(void* value, const std::string& message)
    {
        if (value != nullptr)
        {
            throw std::runtime_error(message);
        }
    }

    void
    isEqual(int value, int expected, const std::string& message)
    {
        if (value != expected)
        {
            throw std::runtime_error(message);
        }
    }
} // namespace Mlib::Assert
