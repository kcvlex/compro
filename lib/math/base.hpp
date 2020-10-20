#pragma once
#include "util/template.hpp"

namespace math {

template <typename T>
constexpr T mul_id_ele() {
    if constexpr (std::is_fundamental<T>::value) {
        return T(1);
    } else {
        return T::mul_id_ele();
    }
}

template <typename T>
constexpr T add_id_ele() {
    if constexpr (std::is_fundamental<T>::value) {
        return T(0);
    } else {
        return T::add_id_ele();
    }
}

template <typename T>
constexpr T pow(const T &n, ll k) {
    T ret = mul_id_ele<T>();
    T cur = n;
    while (k) {
        if (k & 1) ret *= cur;
        cur *= cur;
        k /= 2;
    }
    return ret;
}


template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
gcd(T a, T b) { return b ? gcd(a % b, b) : a; }

}
