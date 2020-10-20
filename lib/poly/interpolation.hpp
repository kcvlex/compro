#pragma once
#include "multipoint-eval.hpp"

namespace poly {

template <typename Poly>
Poly interpolation(const vec<typename Poly::value_type> &xv, const vec<typename Poly::value_type> &yv) {
    SubproductTree<Poly> ptree(xv);
    auto f = ptree.get_poly(ptree.height(), 0);
    f.diff();
    auto z = internal::MultipointEvaluationSolver(&ptree).solve(std::move(f));

    vec<Poly> b(ptree.size());
    for (size_type i = 0; i < size_type(z.size()); i++) b[i] = Poly { yv[i] / z[i] };

    for (size_type i = 0; i < ptree.height(); i++) {
        auto len = ptree.get_len(i);
        for (size_type j = 0; j < len / 2; j++) {
            auto m1 = std::move(mul(ptree.get_poly(i, 2 * j), std::move(b[2 * j + 1])));
            auto m2 = std::move(mul(ptree.get_poly(i, 2 * j + 1), std::move(b[2 * j])));
            b[j] = std::move(add(std::move(m1), std::move(m2)));
        }
        if (len & 1) b[len / 2] = std::move(b[len - 1]);
    }

    return b[0];
}

}
