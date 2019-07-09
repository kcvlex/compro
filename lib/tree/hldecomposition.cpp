#include <bits/stdc++.h>
using namespace std;

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
using ll = int64_t;

struct HeavyLightDecomposition {
    ll root;
    const VV<ll> &edges;
    V<ll> subtree_size, head, orig_parent_idx, hl_id, hl_component_id, heavy_edge_to;

    HeavyLightDecomposition(const VV<ll> &edges)
        : root(0),
          edges(edges),
          subtree_size(edges.size()),
          head(edges.size()),
          orig_parent_idx(edges.size()),
          hl_id(edges.size()),
          hl_component_id(edges.size()),
          heavy_edge_to(edges.size())
    {
    }

    ll count_size(ll cur, ll pre) {
        ll ret = 1;
        orig_parent_idx[cur] = pre;
        ll max_size = 0, max_size_to = -1;
        for(ll nxt : edges[cur]) {
            if(nxt == pre) continue;
            ll res = count_size(nxt, cur);
            if(max_size < res) {
                max_size = res;
                max_size_to = nxt;
            }
            ret += res;
        }
        heavy_edge_to[cur] = max_size_to;
        return subtree_size[cur] = ret;
    }

    ll get_hl_id(ll node) { return node == -1 ? -1 : hl_id[node]; }

    void dfs_hl(ll cur, ll pre, ll &hl_id_counter, ll &total_hl) {
        hl_id[cur] = hl_id_counter++;
        hl_component_id[cur] = total_hl;
        head[cur] = (get_hl_id(pre) == -1 ? cur : head[pre]);

        if(heavy_edge_to[cur] != -1) dfs_hl(heavy_edge_to[cur], cur, hl_id_counter, total_hl);

        for(ll nxt : edges[cur]) {
            if(nxt == pre || nxt == heavy_edge_to[cur]) continue;
            dfs_hl(nxt, cur, hl_id_counter, ++total_hl);
        }
    }

    void decompose() {
        count_size(root, -1);
        ll hl_id_counter = 0;
        ll total_hl = 0;
        dfs_hl(root, -1, hl_id_counter, total_hl);
    }

    // careful : when query handle edges
    template <typename T>
    T query(ll n1, ll n2, function<T(ll, ll)> &calc_component, function<T(T, T)> &merge, T init) {
        T lval = init, rval = init;
        T result = init;
        while(true) {
            if(hl_component_id[n1] != hl_component_id[n2]) {
                if(hl_id[n1] < hl_id[n2]) {
                    T tmp = calc_component(head[n2], hl_id[n2] + 1);
                    rval = merge(tmp, rval);
                    n2 = orig_parent_idx[head[n2]];
                } else {
                    T tmp = calc_component(head[n1], hl_id[n1] + 1);
                    lval = merge(lval, tmp);
                    n1 = orig_parent_idx[head[n1]];
                }
            } else {
                ll id1 = hl_id[n1];
                ll id2 = hl_id[n2];
                result = calc_component(min(id1, id2), max(id1, id2) + 1);
                result = merge(lval, result);
                result = merge(result, rval);
                break;
            }
        }
        return result;
    }
};
