#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include "def.h"

namespace Mlib::Math {
    /// @name clamp
    /// @brief Clamps a value of x to the range [ a, b ].
    /// @param a  The lower bound of the range.
    /// @param b  The upper bound of the range.
    /// @param x  The value to clamp.
    /// @returns f64
    /// - If ( x ) is less than ( a ), ( a ) is returned.
    /// - If ( x ) is greater than ( b ), ( b ) is returned.
    /// - Otherwise, ( x ) is returned.
    /// @throws std::invalid_argument
    /// - If ( a ) is greater than ( b ).
    f64 clamp(f64 a, f64 b, f64 x);

    /// @name between
    /// @brief Checks a value of x to the range [ a, b ].
    /// @param a  The lower bound of the range.
    /// @param b  The upper bound of the range.
    /// @param x  The value to check.
    /// @returns bool
    /// - true   If the value is within the range.
    /// - false  If the value is outside the range.
    /// @throws std::invalid_argument
    /// - If ( a ) is greater than ( b ).
    bool between(f64 a, f64 b, f64 x);
} // namespace Mlib::Math
