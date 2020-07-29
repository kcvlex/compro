#pragma once
#include "../util/template.hpp"
#include "base.hpp"
#include "modint.hpp"

namespace math {

template <typename T>
class Factorial {
    size_type maxv;
    vec<T> factv, ifactv;

public:
    Factorial(ssize_t maxv) : maxv(maxv), factv(maxv + 1), ifactv(maxv + 1) {
        factv[0] = T(1);
        for (ll i = 1; i <= maxv; i++) factv[i] = factv[i - 1] * i;
        ifactv.back() = factv.back().inv();
        for (ll i = maxv - 1; 0 <= i; i--) ifactv[i] = ifactv[i + 1] * (i + 1);
    }

    T fact(size_type n) const {
        return factv[n]; 
    }

    T ifact(size_type n) const {
        return ifactv[n];
    }

    T perm(size_type n, size_type k) const {
        return factv[n] * ifactv[n - k];
    }

    T comb(size_type n, size_type k) const {
        return perm(n, k) * ifactv[k];
    }

    T catalan(size_type n) const {
        return fact[2 * n] * ifact[n + 1] * ifact[n];
    }
};

}
