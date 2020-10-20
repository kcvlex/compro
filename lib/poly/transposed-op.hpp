#pragma once
#include "fps.hpp"

namespace poly {

template <typename Poly>
Poly transposed_mul(Poly a, Poly c) {
    size_type m = a.size();
    size_type n = c.size() - m;
    return a.reverse()
            .mul(std::move(c))
            .middle(m, n + m + 1);
}

}
