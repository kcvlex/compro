#include <bits/stdc++.h>
#include "../util.cpp"
using namespace std;
using ll = int64_t;

template <size_t Size = 30>
class Lca {
    const ll nopt;
    ll N;
    ll root;
    V<ll> depth;
    VV<ll> edge;
    VV<ll> parents;

    void dfs(ll now, ll pre, ll d) {
        parents[now][0] = pre;
        depth[now] = d;
        for(ll next : edge[now]) {
            if(next != pre) {
                dfs(next, now, d + 1);
            }
        }
    }

public:
    Lca(ll N, ll root, const vector<vector<ll>> &e)
        : nopt(-1),
          N(N),
          root(root),
          edge(e),
          parents(N, V<ll>(Size))
    {
        depth.resize((size_t)N);
        dfs(root, nopt, 0);

        for(ll i = 1; i < Size; i++) {
            for(ll node = 0; node < N; node++) {
                if(parents[node][i - 1] == nopt) {
                    parents[node][i] = nopt;
                } else {
                    parents[node][i] = parents[parents[node][i - 1]][i - 1];
                }
            }
        }
    }

    Lca(ll n, const vector<vector<ll>> &e) : Lca(n, 0, e) 
    {
    }

    ll get_depth(ll node) { return depth[node]; }

    ll get_parents(ll node, ll relative_depth) {
        ll ret = node;
        for(ll i = 0; (1 << i) <= relative_depth && ret != -1; i++) {
            if(relative_depth & (1 << i)) {
                ret = parents[ret][i];
            }
        }
        return ret;
    }

    ll get_lca(ll n1, ll n2) {
        ll d = min(depth[n1], depth[n2]);
        ll ok = 0, ng = d + 1;
        ll ret = 0;
        while(abs(ok - ng) > 1) {
            ll mid = (ok + ng) / 2;
            ll p1 = get_parents(n1, depth[n1] - mid);
            ll p2 = get_parents(n2, depth[n2] - mid);
            if(p1 == p2) {
                ok = mid;
                ret = p1;
            } else {
                ng = mid;
            }
        }
        return ret;
    }
};
