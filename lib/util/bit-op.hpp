#pragma once
#include "template.hpp"

namespace utility {

template <typename T>
using validate_integer = typename std::enable_if<std::is_integral<T>::value, ll>::type;

template <typename T>
auto popcount(T n) -> validate_integer<T> {
    return __builtin_popcount(n);
}

// 0 indexed
template <typename T>
auto msb(T n) -> validate_integer<T> {
    return 64 - __builtin_clzll(n) - 1;
}

template <typename T>
constexpr auto ceil_pow2(T s) -> validate_integer<T> {
    ll ret = 1;
    while (ret < s) ret *= 2;
    return ret;
}

}
