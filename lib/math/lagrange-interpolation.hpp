#pragma once
#include "base.hpp"

namespace math {

template <typename Container, typename T = Container::value_type>
vec<T> lagrange_interpolation(const Container &xy) {
    size_type n(xy.size());
    vec<T> ret(n);
    vec<T> buf(n);
    for (auto [ x, y ] : xy) {
        std::fill(ALL(buf), 0);
        buf[0] = 1;
        T coef = 1;
        for (auto [ xi, yi ] : xy) {

        }
    }
}

}
