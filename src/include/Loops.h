#pragma once

#include <SDL2/SDL_rect.h>
#include <algorithm>
#include <cmath>
#include <emmintrin.h> // SSE2 for integer operations
#include <immintrin.h> // For SSE/AVX intrinsics
#include <vector>
#include <xmmintrin.h> // SSE
#include "def.h"

#define __AVX_ATTR  \
    __attribute__(( \
        __always_inline__, __nodebug__, __target__("avx,no-evex512"), __min_vector_width__(256)))

#define ITER_THRUE(__Iter, __Container, __Action)                                      \
    for (auto __Iter = (__Container).begin(); __Iter != (__Container).end(); ++__Iter) \
    {                                                                                  \
        __Action                                                                       \
    }

// Templated function for iterating through a container and performing an action
template <typename Container, typename Action>
void iterate_through(Container &container, Action action)
{
    for (auto &element : container)
    {
        action(element);
    }
}

inline void vectorized_addition(std::vector<float> &a, std::vector<float> &b,
                                std::vector<float> &result)
{
    size_t size = a.size();
    size_t i    = 0;
    // Process 8 elements at a time using AVX
    for (; i <= size - 8; i += 8)
    {
        __m256 va = _mm256_loadu_ps(&a[i]);
        __m256 vb = _mm256_loadu_ps(&b[i]);
        __m256 vr = _mm256_add_ps(va, vb);
        _mm256_storeu_ps(&result[i], vr);
    }
    // Process remaining elements
    for (; i < size; ++i)
    {
        result[i] = a[i] + b[i];
    }
}

#define LOAD_AVX(va, vb, a, b) \
    va = _mm256_load_pd(a);    \
    vb = _mm256_load_pd(b)

/* template <typename Container, typename Function>
void parallel_for_each(Container &container, Function func)
{
    std::for_each(__pstl::execution::par, container.begin(), container.end(), func);
} */

// Template function to perform an action on each element
template <typename Container, typename Action>
void applyActionOnData(Container &container, Action action)
{
    auto  *data = &container[0];
    size_t size = container.size();

    for (size_t i = 0; i < size; ++i)
    {
        action(data[i]);
    }
}

// Template function to perform an action on each element
template <typename Container, typename Action>
void applyActionWData(Container &container, Action action)
{
    size_t size = container.size();
    for (size_t i = 0; i < size; ++i)
    {
        action(container[i]);
    }
}

template <typename NUMERIC_T>
void __inline __attribute((__always_inline__, __nodebug__))
SSE2_vector_addit(const NUMERIC_T *a, const NUMERIC_T *b, NUMERIC_T *r, int n) noexcept
{
    if constexpr (std::is_same<NUMERIC_T, short>::value)
    {
        int simd_width = (128 / (sizeof(short) * 8));
        int i          = 0;
        for (; i <= n - simd_width; i += simd_width)
        {
            __m128i va = _mm_load_si128((__m128i *)&a[i]);
            __m128i vb = _mm_load_si128((__m128i *)&b[0]);
            __m128i vr = _mm_add_epi16(va, vb);
            _mm_store_si128((__m128i *)&r[i], vr);
        }
        for (; i < n; ++i)
        {
            r[i] = a[i] + b[i];
        }
    }
    else if constexpr (std::is_same<NUMERIC_T, double>::value)
    {
        int simd_width = (128 / (sizeof(double) * 8));
        int i          = 0;
        for (; i <= n - simd_width; i += simd_width)
        {
            __m128d va = _mm_load_pd(&a[i]);
            __m128d vb = _mm_load_pd(&b[i]);
            __m128d vr = _mm_add_pd(va, vb);
            _mm_store_pd(&r[i], vr);
        }
        for (; i < n; ++i)
        {
            r[i] = a[i] + b[i];
        }
    }
}

template <typename Op, typename Tail_Op>
static void __inline __attribute((__always_inline__, __nodebug__))
SSE_float_vectorized_operation(const float *a, float *r, int n, Op &&operation,
                               Tail_Op &&tail_operation) noexcept
{
    int i = 0;
    for (; i <= n - sizeof_sse_simd(float); i += sizeof_sse_simd(float))
    {
        __m128 va = _mm_load_ps(&a[i]);
        __m128 vr = std::forward<Op>(operation)(va);
        _mm_store_ps(&r[i], vr);
    }
    for (; i < n; ++i)
    {
        r[i] = std::forward<Tail_Op>(tail_operation)(a[i]);
    }
}

/* Specialized square root function */
static void __inline __attribute__((__always_inline__, __nodebug__))
SSE_float_vectorized_sqrt(const float *a, float *r, int n) noexcept
{
    SSE_float_vectorized_operation(
        a, r, n,
        /* SIMD sqrt for 4 floats at a time */
        [](const __m128 &x) -> __m128 {
            return _mm_sqrt_ps(x);
        },
        /* For tail operation one float at a time. */
        [](const float f) {
            return sqrtf(f);
        });
}

// 'r' will hold the sum of 'a + b'.
static void __inline __attribute((__always_inline__, __nodebug__))
SIMD_SSE_add_4_floats(const float a[4] __attribute__((__aligned__(16))),
                      const float b[4] __attribute__((__aligned__(16))),
                      float       r[4] __attribute__((__aligned__(16)))) noexcept
{
    __m128 va = _mm_load_ps(a);
    __m128 vb = _mm_load_ps(b);
    __m128 vr = _mm_add_ps(va, vb);
    _mm_store_ps(r, vr);
}

// 'r' will hold the sum of 'a + b'.
static void __inline __attribute((__always_inline__, __nodebug__))
AVX_SIMD_add_4_floats(const double a[4] __attribute__((__aligned__(32))),
                      const double b[4] __attribute__((__aligned__(32))),
                      double       r[4] __attribute__((__aligned__(32)))) noexcept
{
    __m256d va = _mm256_load_pd(a);
    __m256d vb = _mm256_load_pd(b);
    __m256d vr = _mm256_add_pd(va, vb);
    _mm256_store_pd(r, vr);
}

static void __inline __attribute((__always_inline__, __nodebug__))
AVX_SIMD_add_4_floats(const double **a __attribute__((__aligned__(32))),
                      const double **b __attribute__((__aligned__(32))),
                      double       **r __attribute__((__aligned__(32)))) noexcept
{
    __m256d va = _mm256_load_pd(*a);
    __m256d vb = _mm256_load_pd(*b);
    __m256d vr = _mm256_add_pd(va, vb);
    _mm256_store_pd(*r, vr);
}

static void __AVX_ATTR AVX_SIMD_add_4_double(double **a __attribute__((__aligned__(32))),
                                             double **b __attribute__((__aligned__(32)))) noexcept
{
    __m256d va = _mm256_load_pd(*a);
    __m256d vb = _mm256_load_pd(*b);
    __m256d vr = _mm256_add_pd(va, vb);
    _mm256_store_pd(*a, vr);
}

/* static bool __inline __attribute((__always_inline__, __nodebug__))
is_object_A_inside_B_sse(const float a[4] __attribute((__aligned__(32))),
                         const float b[4] __attribute((__aligned__(32))))
{
    // Load the bounds of objects A and B into SIMD registers (aligned memory)
    __m128 va = _mm_load_ps(a); // Aligned load
    __m128 vb = _mm_load_ps(b); // Aligned load
    // Compare if A's bounds are greater than or equal to B's minimum bounds
    __m128 cmp_min = _mm_cmpge_ps(va, vb); // Compare x_min_A >= x_min_B, y_min_A >= y_min_B
    // Compare if A's max bounds are less than or equal to B's max bounds
    __m128 cmp_max = _mm_cmple_ps(va, vb); // Compare x_max_A <= x_max_B, y_max_A <= y_max_B
    // Combine the results (using AND operation to check if all conditions are true)
    __m128 cmp_result = _mm_and_ps(cmp_min, cmp_max);
    // Extract the results from the SIMD register into an array
    int result = _mm_movemask_ps(cmp_result);
    // If result is 0b1111 (all comparisons are true), return true
    return result == 0b1111;
} */

static bool __inline __attribute__((__always_inline__, __nodebug__))
is_object_A_inside_B_sse(const float a[4] __attribute__((__aligned__(32))),
                         const float b[4] __attribute__((__aligned__(32))))
{
    // Load the bounds of objects A and B into SIMD registers (aligned memory)
    __m128 va = _mm_load_ps(a); // Aligned load
    __m128 vb = _mm_load_ps(b); // Aligned load
    // Compare if A's min bounds (x_min, y_min) are greater than or equal to B's min bounds (x_min,
    // y_min)
    __m128 cmp_min = _mm_cmpge_ps(va, vb); // Compare A >= B for x_min, y_min
    // Swap the max bounds to compare (x_max, y_max) between A and B
    // We need to shuffle the vector to make sure that we compare x_max_A <= x_max_B and y_max_A <=
    // y_max_B
    __m128 va_max =
        _mm_shuffle_ps(va, va, _MM_SHUFFLE(3, 2, 3, 2)); // Extract x_max and y_max from A
    __m128 vb_max =
        _mm_shuffle_ps(vb, vb, _MM_SHUFFLE(3, 2, 3, 2)); // Extract x_max and y_max from B
    __m128 cmp_max = _mm_cmple_ps(va_max, vb_max);       // Compare A <= B for x_max, y_max
    // Combine the results (using AND operation to check if all conditions are true)
    __m128 cmp_result = _mm_and_ps(cmp_min, cmp_max);
    // Extract the results from the SIMD register into an integer bitmask
    int result = _mm_movemask_ps(cmp_result);
    // If result is 0b1111 (all comparisons are true), return true
    return result == 0b1111;
}

static bool __AVX_ATTR is_object_A_inside_B_avx(const double a[4] __attribute__((__aligned__(32))),
                                                const double b[4] __attribute__((__aligned__(32))))
{
    /* Load all 4 doubles from A (x_min, y_min, x_max, y_max) */
    __m256d va = _mm256_load_pd(a);
    /* Load all 4 doubles from B (x_min, y_min, x_max, y_max) */
    __m256d vb = _mm256_load_pd(b);
    /* Compare if A's min bounds (x_min, y_min) are greater
     * than or equal to B's min bounds (x_min, y_min) */
    __m256d cmp_min = _mm256_cmp_pd(va, vb, _CMP_GE_OQ);
    /* Compare if A's max bounds (x_max, y_max) are less
     * than or equal to B's max bounds (x_max, y_max) */
    __m256d cmp_max = _mm256_cmp_pd(va, vb, _CMP_LE_OQ);
    /* Combine the results (using AND operation to check if all conditions are true) */
    __m256d cmp_result = _mm256_and_pd(cmp_min, cmp_max);
    /* Extract the results from the AVX register into an integer bitmask */
    int result = _mm256_movemask_pd(cmp_result);
    /* If result is 0b1111 (all comparisons are true), return true */
    return result == 0b1111;
}

static bool __AVX_ATTR compare_scalar_to_array_avx(const unsigned int *array, unsigned int scalar,
                                                   size_t size)
{
    // Ensure size is a multiple of 8 (since AVX works with 8 unsigned ints at a time)
    unsigned long aligned_size = size - (size % 8);
    // Broadcast scalar to all elements of the AVX register
    __m256i v_scalar = _mm256_set1_epi32(scalar);
    for (size_t i = 0; i < aligned_size; i += 8)
    {
        // Load 8 integers from the array into an AVX register
        __m256i v_array = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(&array[i]));
        // Compare each element in the array with the scalar
        __m256i v_cmp = _mm256_cmpeq_epi32(v_array, v_scalar);
        // Check if any comparison result is true (non-zero)
        if (_mm256_movemask_epi8(v_cmp))
        {
            return true;
        }
    }
    // Handle any remaining elements (if size is not a multiple of 8)
    for (size_t i = aligned_size; i < size; ++i)
    {
        if (array[i] == scalar)
        {
            return true;
        }
    }
    return false;
}

static void update_rect_simd(__m128 *rect, const __m128 *frect, const __m128 *step,
                             const __m128 *end)
{
    // Load rect and frect values (x, y, w, h) into SIMD registers
    __m128 rect_vals = _mm_load_ps((const float *)rect); // Load rect.x, rect.y, rect.w, rect.h
    __m128 frect_vals =
        _mm_load_ps((const float *)frect);               // Load frect.x, frect.y, frect.w, frect.h

    // Step comparisons: step < 0.0f
    __m128 step_lt_zero = _mm_cmplt_ps(*step, _mm_set1_ps(0.0f)); // Check if step values are < 0.0

    // Condition when step < 0.0 and rect < end
    __m128 rect_lt_end    = _mm_cmplt_ps(rect_vals, *end);
    __m128 use_end_for_lt = _mm_and_ps(step_lt_zero, rect_lt_end); // Combine both conditions

    // Step comparisons: step > 0.0f
    __m128 step_gt_zero = _mm_cmpgt_ps(*step, _mm_set1_ps(0.0f)); // Check if step values are > 0.0

    // Condition when step > 0.0 and rect > end
    __m128 rect_gt_end    = _mm_cmpgt_ps(rect_vals, *end);
    __m128 use_end_for_gt = _mm_and_ps(step_gt_zero, rect_gt_end); // Combine both conditions

    // Use the appropriate values based on the conditions
    __m128 use_end =
        _mm_or_ps(use_end_for_lt, use_end_for_gt); // Either case (step < 0.0 or step > 0.0)
    __m128 updated_rect =
        _mm_blendv_ps(frect_vals, *end, use_end);  // Blend values based on the comparison

    // Store the updated values back into rect
    _mm_store_ps((float *)rect, updated_rect);
}

static void update_rect_avx(__m256 *rect, const __m256 *frect, const __m256 *step,
                            const __m256 *end)
{
    // Load rect and frect values (x, y, w, h) into AVX registers
    __m256 rect_vals = _mm256_load_ps((const float *)rect); // Load rect.x, rect.y, rect.w, rect.h
    __m256 frect_vals =
        _mm256_load_ps((const float *)frect); // Load frect.x, frect.y, frect.w, frect.h

    // Step comparisons: step < 0.0f
    __m256 step_lt_zero =
        _mm256_cmp_ps(*step, _mm256_set1_ps(0.0f), _CMP_LT_OQ); // Check if step values are < 0.0

    // Condition when step < 0.0 and rect < end
    __m256 rect_lt_end    = _mm256_cmp_ps(rect_vals, *end, _CMP_LT_OQ);
    __m256 use_end_for_lt = _mm256_and_ps(step_lt_zero, rect_lt_end); // Combine both conditions

    // Step comparisons: step > 0.0f
    __m256 step_gt_zero =
        _mm256_cmp_ps(*step, _mm256_set1_ps(0.0f), _CMP_GT_OQ); // Check if step values are > 0.0

    // Condition when step > 0.0 and rect > end
    __m256 rect_gt_end    = _mm256_cmp_ps(rect_vals, *end, _CMP_GT_OQ);
    __m256 use_end_for_gt = _mm256_and_ps(step_gt_zero, rect_gt_end); // Combine both conditions

    // Use the appropriate values based on the conditions
    __m256 use_end =
        _mm256_or_ps(use_end_for_lt, use_end_for_gt); // Either case (step < 0.0 or step > 0.0)
    __m256 updated_rect =
        _mm256_blendv_ps(frect_vals, *end, use_end);  // Blend values based on the comparison

    // Store the updated values back into rect
    _mm256_store_ps((float *)rect, updated_rect);
}

/* static bool __inline __attribute((__always_inline__, __nodebug__))
AVX_is_ra_inside_rb(const double a[4], const double b[4]) noexcept
{
    __m256d va = _mm256_load_pd(a);
    __m256d vb = _mm256_load_pd(b);
    // __m256d cmp_min = _mm256_cmpge
}
 */
