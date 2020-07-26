#pragma once
#include "includes.hpp"

template <typename T>
T scan() {
    T t;
    std::cin >> t;
    return t;
}

template <typename T>
vec<T> scan<vec<T>>(size_type n) {
    vec<T> v(n);
    for (auto &&e : v) std::cin >> t;
    return v;
}
