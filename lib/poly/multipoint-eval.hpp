#pragma once
#include "fps.hpp"
#include "subproduct-tree.hpp"

namespace poly {

namespace internal {

template <typename Poly>
struct MultipointEvaluationSolver {
    using value_type = typename Poly::value_type;

    MultipointEvaluationSolver(SubproductTree<Poly> *ptree)
        : ptree(ptree), ret(ptree->size())
    {
    }

    const vec<value_type>& solve(Poly f) {
        dfs(std::move(f));
        return ret;
    }

private:
    const SubproductTree<Poly> *ptree;
    vec<value_type> ret;

    void dfs(const Poly &f) {
        auto r = ceil_pow2(ret.size());
        dfs(f, 0, r, ptree->height(), 0);
    }

    void dfs(Poly f, size_type l, size_type r, size_type rank, size_type idx) {
        if (ret.size() <= l) return;
        if (f.degree() < 64) {
            for (size_type i = l; i < std::min(r, ptree->size()); i++) ret[i] = f((*ptree)[i]);
            return;
        }
        if (r - l == 1) {
            ret[l] = f[0];
            return;
        }
        auto m = (l + r) / 2;
        auto rec = [&](ll nl, ll nr, ll nidx) {
            if (ptree->get_len(rank - 1) <= nidx) return;
            auto g = std::move(mod(f, ptree->get_poly(rank - 1, nidx)));
            dfs(std::move(g), nl, nr, rank - 1, nidx);
        };
        rec(l, m, 2 * idx + 0);
        rec(m, r, 2 * idx + 1);
    }
};

}

template <typename Poly>
struct MultipointEvaluation {
    using value_type = typename Poly::value_type;
    
    template <typename Container>
    MultipointEvaluation(const Container &cs) 
        : ptree(cs), solver(&ptree)
    {
    }

    const vec<value_type>& eval(Poly f) {
        return solver.solve(std::move(f));
    }

private:
    SubproductTree<Poly> ptree;
    internal::MultipointEvaluationSolver<Poly> solver;
};

}
