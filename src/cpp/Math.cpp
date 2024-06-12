#include "../include/Math.h"

namespace Mlib::Math {
    FORCE_INLINE UNUSED auto between(double a, double b, double x) -> double
    {
        return x > b ? x = b : x < a ? x = a : x;
    }
} // namespace Mlib::Math
