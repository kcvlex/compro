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

template <typename EdgeAttr>
struct Graph {
    size_t node_num;
    VV<ll> adj_list;
    VV<EdgeAttr> edge_attr;
    
    Graph(size_t node_num) 
        : node_num(node_num), 
          adj_list(node_num),
          edge_attr(node_num) 
    {
    }

    Graph() : Graph(0) {}

    template <typename Attr...>
    void add_edge__(ll x, ll y, Attr&&... attr) {
        EdgeAttr edge(y, forward<Attr>(attr)...);
        adj_list[x].push_back(y);
        edge_attr[x].emplace_back(forward<Attr>(attr)...);
    }

    template <typename Attr...>
    void add_edge(ll x, ll y, bool directed, Attr&&... attr) {
        add__edge(x, y, forward<Attr>(attr)...);
        if(!directed) add_edge__(y, x, forward<Attr>(attr)...);
    }

    size_t size() const { return node_num; }

    ll operator [](size_t idx) { return adj_list[idx]; }
};
