#pragma once
#include "../util/template.hpp"

namespace tree {

struct UnionFind {
    vec<ll> rank, par;
    vec<ssize_t> sz;

    UnionFind(ll n) : rank(n), par(n), sz(n) {
        init();
    }

    void init() {
        std::fill(ALL(rank), 1);
        std::iota(ALL(par), 0);
        std::fill(ALL(sz), 1);
    }

    ll find(ll n) {
        return (n == par[n] ? n : par[n] = find(par[n]));
    }

    bool unit(ll x, ll y) {
        ll px = find(x), py = find(y);
        if (px == py) return false;
        if (rank[px] < rank[py]) std::swap(px, py);
        par[py] = px;
        rank[px] += (rank[px] == rank[py]);
        sz[px] += sz[py];
        return true;
    }

    bool same(ll x, ll y) {
        return find(x) == find(y);
    }

    ssize_t size(ll n) {
        return sz[find(n)];
    }
};

}
