#include "../include/Math.h"

namespace Mlib::Math {
    f64 between(f64 a, f64 b, f64 x)
    {
        return x > b ? x = b : x < a ? x = a : x;
    }
} // namespace Mlib::Math
