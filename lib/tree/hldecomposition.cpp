#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define ALL(V) (V).begin(), (V).end()
#define ALLR(V) (V).rbegin(), (V).rend()

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
using ll = int64_t;
using ull = uint64_t;
using PLL = pair<ll, ll>;

template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return max(t, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }
template <typename T> const T& clamp(const T &t, const T &low, const T &high) { return max(low, min(high, t)); }
template <typename T> void chclamp(T &t, const T &low, const T &high) { t = clamp(t, low, high); }

template <typename T>
T make_v(T init) { return init; }

template <typename T, typename... Tail>
auto make_v(T init, size_t s, Tail... tail) {
#define rec make_v(init, tail...)
    return V<decltype(rec)>(s, rec);
#undef rec
}

struct HeavyLightDecomposition {
    ll root;
    const VV<ll> &edges, hl_edges;
    V<ll> subtree_size, head, orig_parent_idx, hl_id, hl_component_id, hl_parent_id, hl_edges, heavy_edge_to;

    HeavyLightDecomposition(const VV<ll> &edges)
        : root(0),
          total_hl(0),
          edges(edges),
          subree_size(edges.size()),
          head(edges.size()),
          orig_parent(edges.size()),
          hl_id(edges.size()),
          hl_component_id(edges.size()),
          hl_parent_id(edges.size()),
          hl_edges(edges.size()),
          heavy_edge_to(edges.size()),
    {
    }

    ll count_size(ll cur) {
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
        hl_component_id[cur] = component_id++;

        if(get_hl_id(pre) == cur_hl_id) {
            head[cur] = head[pre];
            hl_parent_id[cur] = hl_parent_id[pre];
        } else {
            head[cur] = cur;
            hl_parent_id[cur] = get_hl_id(pre);
        }

        if(heavy_edge_to[cur] != -1) dfs_hl(heavy_edge_to[cur], cur, hl_id_counter, total_hl);

        for(ll nxt : edges[cur]) {
            if(nxt == pre || nxt == heavy_edge_to[cur]) continue;
            dfs_hl(nxt, cur, hl_id_counter, total_hl);
        }
    }

    void decompose() {
        count_size(root, -1);
        ll hl_id_counter = 0;
        ll total_hl = 0;
        dfs_hl(root, -1, hl_id_counter, total_hl);
    }

    template <typename T>
    T node_query(ll n1, ll n2, function<T(ll, ll)> &calc_component, function<T(T, T)> &merge, T init) {
        T lval = init, rval = init;
        T result = init;
        while(true) {
            if(hl_component_id[n1] != hl_component_id[n2]) {
                if(hl_id[n1] < hl_id(n2)) {
                    T tmp = calc_component(head[n2], hl_id[n2] + 1);
                    rval = merge(tmp, rval);
                    n2 = orig_parent[head[n2]];
                } else {
                    T tmp = calc_component(head[n1], hl_id[n1] + 1);
                    lval = merge(lval, tmp);
                    n1 = orig_parent[head[n1]];
                }
            } else {
                ll id1 = hl_id[n1];
                ll id2 = hl_id[n2];
                result = merge(lval, merge(calc_components(min(id1, id2), max(id1, id2) + 1)));
                break;
            }
        }
        return result;
    }
};
