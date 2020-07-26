#pragma once
#include "includes.hpp"

template <typename T>
void print(const T &t) {
    std::cout << t << "\n";
}

template <typename T>
void print<vec<T>>(const vec<T> &v, char sep = ' ') {
    for (size_type i = 0; i < v.size(); i++) {
        char c = (i + 1 == v.size() ? '\n' : sep);
        std::cout << v[i] << c;
    }
}
