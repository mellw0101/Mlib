#pragma once

/// @file Assert.h
/// @brief Contains the definition of the Assert class.
/// @note This file is included in the Assert.cpp file.

#include <iostream>
#include <stdexcept>
#include <string>

#include "def.h"

#define ASSERT_STATEMENT(x) x ? void(0) : throw std::runtime_error("Assertion failed: " #x)

namespace Mlib::Assert {
    /// @brief Asserts that the given condition is true.
    /// @param condition The condition to assert.
    /// @param message The message to display if the condition is false.
    /// @note If the condition is false, this function will throw a std::runtime_error.
    void isTrue(bool condition, const std::string& message);

    /// @brief Asserts that the given condition is false.
    /// @param condition The condition to assert.
    /// @param message The message to display if the condition is true.
    /// @note If the condition is true, this function will throw a std::runtime_error.
    void isFalse(bool condition, const std::string& message);

    /// @brief Asserts that the given value is not null.
    /// @param value The value to assert.
    /// @param message The message to display if the value is null.
    /// @note If the value is null, this function will throw a std::runtime_error.
    void isNotNull(void* value, const std::string& message);

    /// @brief Asserts that the given value is null.
    /// @param value The value to assert.
    /// @param message The message to display if the value is not null.
    /// @note If the value is not null, this function will throw a std::runtime_error.
    void isNull(void* value, const std::string& message);

    /// @brief Asserts that the given value is equal to the expected value.
    /// @param value The value to assert.
    /// @param expected The expected value.
    /// @param message The message to display if the value is not equal to the expected value.
    /// @note If the value is not equal to the expected value, this function will throw a std::runtime_error.
    void isEqual(int value, int expected, const std::string& message);
} // namespace Mlib::Assert
