#pragma once
#include "../util/template.cpp"
#include "graph.cpp"

namespace graph {

template <std::size_t Size>
class IndependentSet {
    constexpr static std::size_t half = (Size + 1) / 2;
    constexpr static std::size_t hsz = (1ll << half);
    vvec<ll> dp;
    const std::size_t gsz;
    std::array<ll, Size> bs_graph;

    void build_neigh(std::array<ll, hsz> &gr, std::size_t sz) {
        for (ll i = 0; i < sz; i++) {
            ll mask = 1ll << i;
            for (ll s = 0; s < (1ll << sz); s++) if (s & mask) gr[s] |= gr[s ^ mask];
        }
    }

    void calc_sz(vec<ll> &sz_v, std::array<ll, hsz> &gr, std::size_t sz) {
        std::fill(ALL(sz_v), 0);
        build_neigh(gr, sz);
        for (ll s = 0; s < sz_v.size(); s++) {
            ll e = gr[s];
            if (s & e) continue;
            sz_v[s] = std::bitset<64>(s).count();
        }
        for (ll i = 0; i < sz; i++) {
            ll mask = 1ll << i;
            for (ll s = 0; s < (1ll << sz); s++) if (s & mask) chmax(sz_v[s], sz_v[s ^ mask]);
        }
    }

    void extract_graph(std::array<ll, hsz> &arr, ll l, ll r, ll tl, ll tr) {
        std::fill(ALL(arr), 0);
        std::size_t s = tr - tl;
        ll mask = (1ll << s) - 1;
        for (ll i = 0; i < r - l; i++) arr[1ll << i] = (bs_graph[i + l] >> tl) & mask;
    }

public:
    template <bool Dir>
    IndependentSet(const graph::Graph<Dir> &g) : gsz(g.size()), dp(2) {
        std::fill(ALL(bs_graph), 0);
        for (ll i = 0; i < gsz; i++) for (auto &&e : g[i]) {
            ll nxt = e.first;
            bs_graph[i] |= (1ll << nxt);
        }
    }

    ll solve() {
        ll left = 0, mid = gsz / 2, right = gsz;
        pii ranges[] = { { left, mid, }, { mid, right, } };
        std::array<ll, hsz> halfg;

        for (ll i = 0; i < 2; i++) {
            ll l, r;
            std::tie(l, r) = ranges[i];

            std::size_t sz = r - l;
            dp[i].resize(1ll << sz);
            extract_graph(halfg, l, r, l, r);
            calc_sz(dp[i], halfg, sz);
        }

        extract_graph(halfg, left, mid, mid, right);
        build_neigh(halfg, mid - left);
        ll mask = 1ll << (right - mid);
        for (ll i = 0; i < (1ll << (mid - left)); i++) {
            ll &e = halfg[i];
            e = (~e) & (mask - 1);
        }

        ll ret = 0;
        for (ll i = 0; i < (1ll << (mid - left)); i++) {
            ll j = halfg[i];
            chmax(ret, dp[0][i] + dp[1][j]);
        }

        return ret;
    }
};

template <bool Dir>
ll indep_set(const Graph<Dir> &gr) {
    IndependentSet<40> iset(gr);
    return iset.solve();
}

}
