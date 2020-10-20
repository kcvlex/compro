#pragma once

template <typename T>
T ceil_pow2(T bound) {
    T ret = 1;
    while (ret < bound) ret *= 2;
    return ret;
}
