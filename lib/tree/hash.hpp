#pragma once
#include "../math/modint.hpp"
#include "../graph/graph.hpp"

namespace tree {

template <ll Mod, typename Graph>
struct RootedTreeHash {
    using mint = math::Modint<Mod>;

    RootedTreeHash(const Graph &g, const vec<mint> &rv) 
        : g(g), dist(g.size()), rv(rv), maxd(g.size()), base_v(g.size())
    {
        hash_v[0] = vec<mint>(g.size());
        hash_v[1] = vec<mint>(g.size());
        build();
    }

    mint operator [](size_type i) const {
        return hash_v[0][i];
    }

    mint remove_leaf(ll leaf) const {
        mint v = rv[maxd[leaf].first.first];
        return hash_v[0][leaf] - v;
    }

private:
    const Graph &g;
    using max_dist_type = std::pair<pll, pll>;
    vec<ll> dist;
    const vec<mint> &rv;
    vec<max_dist_type> maxd;
    vec<mint> base_v, hash_v[2];

    size_type dfs_rv(ll cur, ll pre) {
        size_type ret = 0;
        for (ll nxt : graph::dst(g[cur])) if (nxt != pre) chmax(ret, dfs_rv(nxt, cur) + 1);
        return dist[cur] = ret;
    }

    mint dfs_base(ll cur, ll pre) {
        mint ret = 1;
        for (ll nxt : graph::dst(g[cur])) if (nxt != pre) {
            auto res = dfs_base(nxt, cur);
            ret *= res + rv[dist[cur]];
        }
        return base_v[cur] = ret;
    }

    void update_dist(size_type i, pll p) {
        auto [ a, b ] = maxd[i];
        if (a < p) {
            b = a;
            a = p;
        } else if (b < p) {
            b = p;
        }
        maxd[i] = std::make_pair(a, b);
    }

    void dfs_dist(ll cur, ll pre, ll ph = -1) {
        maxd[cur] = std::make_pair(pll(0, cur), pll(-1, -1));

        for (ll nxt : graph::dst(g[cur])) {
            ll d = (nxt == pre ? ph : dist[nxt]) + 1;
            update_dist(cur, pll(d, nxt));
        }

        auto [ a, b ] = maxd[cur];
        for (ll nxt : graph::dst(g[cur])) if (nxt != pre) {
            ll d = (a.second == nxt ? b.first : a.first);
            dfs_dist(nxt, cur, d);
        }
    }

    void rerooting(ll cur, ll pre, mint pv, mint phash) {
        auto [ a, b ] = maxd[cur];
        if (pre != -1) {
            ll hlis[] = { a.first, b.first };
            for (int i = 0; i < 2; i++) {
                mint cv = rv[hlis[i]];
                mint hv = base_v[cur];
                mint hash = cv + (phash / (pv + hv));
                for (ll nxt : graph::dst(g[cur])) if (nxt != pre) hash *= cv + base_v[nxt];
                hash_v[i][cur] = hash;
            }
        } else {
            hash_v[0][cur] = base_v[cur];
            hash_v[1][cur] = 1;
            for (ll nxt : graph::dst(g[cur])) hash_v[1][cur] *= rv[b.first] + base_v[nxt];
        }
        
        for (ll nxt : graph::dst(g[cur])) if (nxt != pre) {
            if (a.second == nxt) {
                rerooting(nxt, cur, rv[b.first], hash_v[1][cur]);
            } else {
                rerooting(nxt, cur, rv[a.first], hash_v[0][cur]);
            }
        }
    }

    void build() {
        ll root = 0;
        dfs_rv(root, -1);
        dfs_base(root, -1);
        dfs_dist(root, -1);
        rerooting(root, -1, 0, 0);
    }
};

template <ll Mod, typename Graph>
auto tree_hash(const Graph &g) {
    using mint = math::Modint<Mod>;
    static vec<mint> rv = { 1 };
    static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    // static std::mt19937_64 rng(0);
    
    while (rv.size() < g.size()) {
        ll a = std::uniform_int_distribution<ll>(0, Mod - 1)(rng);
        rv.emplace_back(a);
    }

    return RootedTreeHash<Mod, Graph>(g, rv);
}

}
