#pragma once
#include "template.hpp"

namespace utility {

template <typename T>
using validate_integer = typename std::enable_if<std::is_integral<T>::value, ll>::type;

template <typename T>
auto popcount(T n) -> typename validate_integer<T> {
    return __builtin_popcount(n);
}

// 0 indexed
template <typename T>
auto msb(T n) -> typename validate_integer<T> {
    return 32 - __builtin_clz(i) - 1;
}

template <typename T>
auto ceil_pow2(T s) -> typename validate_integer<T> {
    ll ret = 1;
    while (ret < s) ret *= 2;
    return ret;
}

template <typename T>
auto ceil_log2(ll n) -> typename validate_integer<T> {
    ll m = msb(n);
    if (n == (1ll << m)) return m;
    else return m + 1;
}

}
