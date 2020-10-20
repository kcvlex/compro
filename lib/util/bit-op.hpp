#pragma once
#include "template.hpp"

namespace utility {

template <typename T>
using validate_integer = typename std::enable_if<std::is_integral<T>::value, ll>::type;

template <typename T>
auto popcount(T n) -> validate_integer<T> {
    return __builtin_popcountll(n);
}

// 0 indexed
template <typename T>
auto msb(T n) -> validate_integer<T> {
    return 64 - __builtin_clzll(n) - 1;
}

}
