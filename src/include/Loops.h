#pragma once

#include <algorithm>
#include <immintrin.h> // For SSE/AVX intrinsics
#include <vector>

#define ITER_THRUE(__Iter, __Container, __Action)                                      \
    for (auto __Iter = (__Container).begin(); __Iter != (__Container).end(); ++__Iter) \
    {                                                                                  \
        __Action                                                                       \
    }

// Templated function for iterating through a container and performing an action
template <typename Container, typename Action>
void
iterate_through(Container &container, Action action)
{
    for (auto &element : container)
    {
        action(element);
    }
}

inline void
vectorized_addition(std::vector<float> &a, std::vector<float> &b, std::vector<float> &result)
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

template <typename Container, typename Function>
void
parallel_for_each(Container &container, Function func)
{
    std::for_each(__pstl::execution::par, container.begin(), container.end(), func);
}

// Template function to perform an action on each element
template <typename Container, typename Action>
void
applyActionOnData(Container &container, Action action)
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
void
applyActionWData(Container &container, Action action)
{
    size_t size = container.size();

    for (size_t i = 0; i < size; ++i)
    {
        action(container[i]);
    }
}
