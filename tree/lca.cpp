#include <bits/stdc++.h>
#include "../util.cpp"
using namespace std;
using ll = int64_t;

struct Lca {
    size_t nodes;
    ll root;
    V<ll> depth;
    VV<ll> parents;

    void dfs(ll now, ll pre, ll d, const VV<ll> &edges) {
        parents[now][0] = pre;
        depth[now] = d;
        for (ll next : edge[now]) if (next != pre) dfs(next, now, d + 1, edges);
    }

    Lca(size_t nodes, const VV<ll> &edges, ll root = 0) 
        : nodes(nodes),
          root(root),
          parents(make_v<ll>(-1, N, 30))
    {
        dfs(root, -1, 0, edges);
        for (ll dig = 1; dig < 30; dig++) for (ll i = 0; i < nodes; i++) {
            ll par = parents[i][dig - 1];
            parents[i][dig] = (par == -1 ? -1 : parents[par][dig - 1]);
        }
    }

    ll get_depth(ll node) { return depth[node]; }

    ll get_parent(ll node, ll relative_depth) {
        ll ret = node;
        for (ll i = 0; relative_depth && ret != -1; i++, relativ_depth /= 2) if (relative_depth & 1) ret = parents[ret][i];
        return ret;
    }

    ll get_lca(ll n1, ll n2) {
        if (depth[n1] < depth[n2]) swap(n1, n2);
        ll diff = depth[n1] - depth[n2];
        n1 = get_parents(n1, diff);
        if (n1 == n2) return n1;
        for (ll dig = 29; 0 <= dig; dig--) {
            ll pn1 = get_parent(n1, 1ll << dig);
            ll pn2 = get_parent(n2, 1ll << dig);
            if (pn1 != pn2) tie(n1, n2) = PLL(pn1, pn2);
        }
        return parents[pn1][0];
    }
};
