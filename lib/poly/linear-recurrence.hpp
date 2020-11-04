#pragma once
#include "poly/fps.hpp"

namespace poly {

// Berlekamp-Massey algorithm
template <typename Poly>
Poly find_linear_recurrence(const vec<typename Poly::value_type> &s) {
    using value_type = typename Poly::value_type;
    Poly C { 1 };
    Poly B { 1 };
    value_type b = 1;
    size_type x = 1, L = 0, sz = size_type(s.size());
    for (ll N = 0; N < sz; N++) {
        value_type d = s[N];
        for (size_type i = 1; i <= std::min(L, C.degree()); i++) d += C[i] * s[N - i];

        if (d.value() == 0) {
            x++;
            continue;
        }

        value_type coef = d / b;
        if (N < 2 * L) {
            for (size_type i = 0; i <= B.degree(); i++) C[i + x] -= coef * B[i];
            x++;
        } else {
            Poly tmp = C;
            C.extend(B.degree() + x);
            for (size_type i = 0; i <= B.degree(); i++) C[i + x] -= coef * B[i];
            L = N + 1 - L;
            B = std::move(tmp);
            b = d;
            x = 1;
        }
    }
    C.erase(C.begin());
    return C.mul(-1);
}

}
