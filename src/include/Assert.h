/// @file Assert.h
#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "def.h"

using namespace std;

#define ASSERT_STATEMENT(x) x ? void(0) : throw std::invalid_argument("Assertion failed: " #x)

namespace Mlib::Assert
{
    //
    /// @brief
    ///  -  Asserts that the given condition is true.
    ///
    /// @param condition
    ///  -  The condition to assert.
    ///
    /// @param message
    ///  -  The message to display if the condition is false.
    ///
    /// @note
    ///  -  If the condition is false, this function will throw a std::runtime_error.
    ///
    /// @returns
    ///  -  void
    //
    void isTrue(bool condition, const string &message);

    /// @brief Asserts that the given condition is false.
    /// @param condition The condition to assert.
    /// @param message The message to display if the condition is true.
    /// @note If the condition is true, this function will throw a std::runtime_error.
    void isFalse(bool condition, const string &message);

    /// @brief Asserts that the given value is not null.
    /// @param value The value to assert.
    /// @param message The message to display if the value is null.
    /// @note If the value is null, this function will throw a std::runtime_error.
    void isNotNull(void *value, const string &message);

    /// @brief Asserts that the given value is null.
    /// @param value The value to assert.
    /// @param message The message to display if the value is not null.
    /// @note If the value is not null, this function will throw a std::runtime_error.
    void isNull(void *value, const string &message);

    /// @brief Asserts that the given value is equal to the expected value.
    /// @param value The value to assert.
    /// @param expected The expected value.
    /// @param message The message to display if the value is not equal to the expected value.
    /// @note If the value is not equal to the expected value, this function will throw a std::runtime_error.
    void isEqual(int value, int expected, const string &message);
} // namespace Mlib::Assert
