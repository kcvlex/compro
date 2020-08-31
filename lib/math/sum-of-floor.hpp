#pragma once
#include "../util/template.hpp"

namespace math {

// sum of floor((a * i + b) / m) for i \in [0, n)
// https://judge.yosupo.jp/submission/147
template <typename T>
T sum_of_floor(T n, T m, T a, T b) {
    T ret = 0;

    if (m <= a) {
        T sum = n * (n - 1) / 2;
        ret += (a / m) * sum;
        a %= m;
    }

    if (m <= b) {
        ret += (b / m) * n;
        b %= m;
    }

    T y_max = (a * n + b) / m;
    T x_max = y_max * m - b;
    if (y_max == 0) return ret;
    ret += (n - ceil_div(x_max, a)) * y_max;
    ret += sum_of_floor(y_max, a, m, (a - x_max % a) % a);
    return ret;
}

}
