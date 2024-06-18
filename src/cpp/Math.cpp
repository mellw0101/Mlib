/// @file Math.cpp
#include "../include/Math.h"
#include "../include/Assert.h"

using namespace std;

namespace Mlib::Math {
    f64
    clamp(f64 a, f64 b, f64 x)
    {
        ASSERT_STATEMENT(a < b);
        return x > b ? x = b : x < a ? x = a : x;
    }

    bool
    between(f64 a, f64 b, f64 x)
    {
        ASSERT_STATEMENT(a < b);
        return x > b ? false : x < a ? false : true;
    }

    f64
    calculateBounceVelocity(f64 initialVelocity, f64 restitutionCoefficient)
    {
        return -initialVelocity * restitutionCoefficient;
    }
} // namespace Mlib::Math
