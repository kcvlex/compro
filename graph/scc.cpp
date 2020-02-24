#include "../util/template.cpp"
#include "graph.cpp"

namespace graph {

template <typename Graph>
class StronglyConnectedComponents {
    const Graph &graph;
    Graph rgraph;
    vec<ll> label;
    vec<ll> result;

    void dfs1(ll cur, ll &l) {
        label[cur] = -2;
        for (const graph::Edge &e : graph[cur]) {
            ll nxt;
            std::tie(nxt, std::ignore) = e;
            if (label[nxt] != -1) continue;
            dfs1(nxt, l);
        }
        label[cur] = l++;
    }

    void write_label() {
        ll l = 0;
        for (ll i = 0; i < graph.size(); i++) if (label[i] == -1) dfs1(i, l);
    }

    void dfs2(ll cur, ll l) {
        result[cur] = l;
        for (const graph::Edge &e : rgraph[cur]) {
            ll nxt;
            std::tie(nxt, std::ignore) = e;
            if (result[nxt] != -1) continue;
            dfs2(nxt, l);
        }
    }

    void build_scc() {
        ll l = 0;
        vec<ll> ord(rgraph.size());
        std::iota(ALL(ord), 0ll);
        std::sort(ALL(ord), [&](ll i, ll j) { return label[i] > label[j]; });
        for (ll n : ord) if (result[n] == -1) dfs2(n, l++);
    }

public:
    StronglyConnectedComponents(const Graph &graph)
        : graph(graph), label(graph.size(), -1), result(graph.size(), -1) 
    {
        rgraph = graph.build_inv();
    }

    vec<ll> build() {
        write_label();
        build_scc();
        return result;
    }
};

}

using SCC = graph::StronglyConnectedComponents<graph::Graph<true>>;
