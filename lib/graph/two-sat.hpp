#pragma once
#include "util/template.hpp"
#include "scc.hpp"
#include "graph.hpp"

namespace graph {

namespace sat {

vec<bool> allocation(const vec<ll> &cmp) {
    assert(cmp.size() % 2 == 0);
    ll sz = cmp.size() / 2;
    vec<bool> ret(sz);
    for (ll i = 1; i < sz; i++) ret[i] = (cmp[i + sz] < cmp[i]);
    return ret;
}

}

struct TwoSAT {
    TwoSAT(size_type n) : sz(n), g(2 * (n + 1)), cmp() { }

    // 1 <= |u|, |v| < sz
    void add_cond(ll u, ll v) {
        if (u == v) {
            g.add_edge(fix_neg(-u), fix_neg(u));
        } else if (u == -v) {
            // nothing
        } else {
            g.add_edge(fix_neg(-u), fix_neg(v));
            g.add_edge(fix_neg(-v), fix_neg(u));
        }
    }

    auto solve() -> std::pair<bool, vec<bool>> {
        cmp = std::move(scc(g));
        ll offset = cmp.size() / 2;
        for (ll i = 1; i <= sz; i++) if (cmp[i] == cmp[i + offset]) return std::make_pair(false, vec<bool>());
        return std::make_pair(true, std::move(sat::allocation(cmp)));
    }

    const auto& raw() const {
        return cmp;
    }

private:
    const size_type sz;
    graph::Graph<true> g;
    vec<ll> cmp;

    ll fix_neg(ll v) {
        if (v < 0) v = sz + 1 - v;
        return v;
    }
};

}
