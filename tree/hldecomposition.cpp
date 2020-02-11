#include <bits/stdc++.h>
using namespace std;

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
using ll = int64_t;

template <typename Graph>
struct HeavyLightDecomposition {
    ll root;
    const Graph &graph;
    V<ll> subtree_size, head, parent_node_idx, decomposed_id, component_id, heavy_edge_to;

    HeavyLightDecomposition(const Graph &graph)
        : root(0),
          graph(graph),
          subtree_size(graph.size()),
          head(graph.size()),
          parent_node_idx(graph.size()),
          decomposed_id(graph.size()),
          component_id(graph.size()),
          heavy_edge_to(graph.size())
    {
    }

    ll count_size(ll cur, ll pre) {
        ll ret = 1;
        parent_node_idx[cur] = pre;
        ll max_size = 0, max_size_to = -1;
        for (ll nxt : graph[cur]) {
            if (nxt == pre) continue;
            ll res = count_size(nxt, cur);
            if (max_size < res) {
                max_size = res;
                max_size_to = nxt;
            }
            ret += res;
        }
        heavy_edge_to[cur] = max_size_to;
        return subtree_size[cur] = ret;
    }

    ll get_decomposed_id(ll node) { return node == -1 ? -1 : decomposed_id[node]; }

    void build_component(ll cur, ll pre, ll &decomposed_id_counter, ll &total_hl) {
        decomposed_id[cur] = decomposed_id_counter++;
        component_id[cur] = total_hl;
        head[cur] = (get_decomposed_id(pre) == -1 ? cur :
                     component_id[cur] == component_id[pre] ? head[pre] : cur);

        if (heavy_edge_to[cur] != -1) build_component(heavy_edge_to[cur], cur, decomposed_id_counter, total_hl);

        for (ll nxt : graph[cur]) {
            if (nxt == pre || nxt == heavy_edge_to[cur]) continue;
            build_component(nxt, cur, decomposed_id_counter, ++total_hl);
        }
    }

    void decompose() {
        count_size(root, -1);
        ll decomposed_id_counter = 0;
        ll total_hl = 0;
        build_component(root, -1, decomposed_id_counter, total_hl);
    }

    // careful : when query handle edges
    template <typename T>
    T query(ll n1, ll n2, const function<T(ll, ll)> &calc_component, T identity, const function<T(T, T)> &merge) {
        T lval = identity, rval = identity;
        T result = identity;
        while (true) {
            if (component_id[n1] != component_id[n2]) {
                if (decomposed_id[n1] < decomposed_id[n2]) {
                    T tmp = calc_component(decomposed_id[head[n2]], decomposed_id[n2] + 1);
                    rval = merge(tmp, rval);
                    n2 = parent_node_idx[head[n2]];
                } else {
                    T tmp = calc_component(decomposed_id[head[n1]], decomposed_id[n1] + 1);
                    lval = merge(lval, tmp);
                    n1 = parent_node_idx[head[n1]];
                }
            } else {
                ll id1 = decomposed_id[n1];
                ll id2 = decomposed_id[n2];
                result = calc_component(min(id1, id2), max(id1, id2) + 1);
                result = merge(lval, result);
                result = merge(result, rval);
                break;
            }
        }
        return result;
    }

    void query(ll n1, ll n2, const function<void(ll, ll)> &calc_component) {
        ll identity = 0;
        auto merge = [&](ll a, ll b) { return 0; };
        auto wrapper_calc = [&](ll a, ll b) { calc_component(a, b); return 0; };
        query<ll>(n1, n2, wrapper_calc, identity, merge);
    }
};

// solution for https://yukicoder.me/problems/no/399

#include "../segtree/lazysegmenttree.cpp"

VV<ll> edges;

#define VERIFY
#ifdef VERIFY

int main() {
    ll N;
    cin >> N;
    edges.resize(N);
    for (ll i = 1; i < N; i++) {
        ll u, v;
        cin >> u >> v;
        u--; v--;
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    HeavyLightDecomposition<VV<ll>> hld(edges);
    hld.decompose();
    LazySegmentTree<ll, ll> lst(V<ll>(N, 1), 0, 0,
                                [](ll a, ll b) { return a + b; },
                                [](ll a, ll b) { return a + b; },
                                [](ll a, ll b) { return a + b; },
                                [](ll l, ll r, ll v) { return (r - l) * v; },
                                [](ll v) { return v / 2; });

    auto update = [&](ll a, ll b) { lst.update_query(a, b, 1); };
    auto calc_tax = [&](ll a, ll b) { return lst.get_query(a, b); };
    auto merge = [&](ll a, ll b) { return a + b; };

    ll Q;
    cin >> Q;
    ll ans = 0;
    while (Q--) {
        ll a, b;
        cin >> a >> b;
        a--; b--;
        ll val = hld.query<ll>(a, b, calc_tax, 0, merge);
        ans += val;
        hld.query(a, b, update);
    }
    cout << ans << endl;
    return 0;
}

#endif
