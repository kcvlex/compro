#pragma once
#include "util/template.hpp"
#include "graph.hpp"

namespace graph {

template <typename Graph>
class StronglyConnectedComponents {
    const Graph &g;
    Graph rg;
    vec<ll> cmp, vs;
    vec<bool> used;

    void dfs(ll cur) {
        used[cur] = true;
        for (auto nxt : graph::dst(g[cur])) if (!used[nxt]) dfs(nxt);
        vs.push_back(cur);
    }

    void rdfs(ll cur, ll k) {
        cmp[cur] = k;
        used[cur] = true;
        for (auto nxt : graph::dst(rg[cur])) if (!used[nxt]) rdfs(nxt, k);
    }

public:
    StronglyConnectedComponents(const Graph &arg) 
        : g(arg), cmp(arg.size()), vs(), used(arg.size())
    {
        rg = std::move(g.build_inv());
    }

    vec<ll> build() {
        for (ll i = 0; i < int(g.size()); i++) if (!used[i]) dfs(i);
        std::reverse(ALL(vs));
        std::fill(ALL(used), false);
        ll k = 0;
        for (ll v : vs) if (!used[v]) rdfs(v, k++);
        return cmp;
    }
};

template <typename Graph>
vec<ll> scc(const Graph &g) {
    StronglyConnectedComponents<Graph> s(g);
    return s.build();
}

}
