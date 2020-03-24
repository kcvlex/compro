#pragma once
#include "../util/template.cpp"

namespace tree {

struct UnionFind {
    vec<ll> rank, par;
    vec<ssize_t> sz;

    UnionFind(ll n) : rank(n, 1), par(n), sz(n, 1) {
        iota(ALL(par), 0);
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
