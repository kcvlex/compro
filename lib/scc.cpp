#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;
template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;

class StronglyConnectedComponents {
    VV<ll> E, inv_E;
    V<ll> number;
    V<bool> used_dfs;
    ll to_write_num = 0;

    void dfs(ll now) {
        used_dfs[now] = 1;
        for(ll next : E[now]) {
            if(used_dfs[next]) {
                continue;
            }
            dfs(next);
        }
        number[now] = to_write_num++;
    }

    void dfs(ll now, V<bool> &used, vector<ll> &ret) {
        used[now] = 1;
        ret.push_back(now);
        for(ll next : inv_E[now]) {
            if(used[next]) {
                continue;
            }
            dfs(next, used, ret);
        }
    }

    void write_num() {
        for(ll i = 0; i < E.size(); i++) {
            if(!used_dfs[i]) {
                dfs(i);
            }
        }
    }

public:
    StronglyConnectedComponents(const VV<ll> &E) 
        : E(E),
          inv_E(E.size(), V<ll>(0)),
          number(E.size()),
          used_dfs(E.size(), false)
    {
        for(ll i = 0; i < E.size(); i++) {
            for(ll nxt : E[i]) {
                inv_E[nxt].push_back(i);
            }
        }
        write_num();
    }

    VV<ll> scc() {
        VV<ll> ret;
        V<ll> vertex_vec;
        V<bool> used(E.size());
        for(ll i = 0; i < E.size(); i++) {
            vertex_vec.push_back(i);
        }
        sort(vertex_vec.begin(), vertex_vec.end(), [&](ll a, ll b) { return number[a] > number[b]; });
        for(ll vertex : vertex_vec) {
            if(used[vertex]) {
                continue;
            }
            V<ll> to_insert;
            dfs(vertex, used, to_insert);
            ret.push_back(to_insert);
        }
        return ret;
    }
};
