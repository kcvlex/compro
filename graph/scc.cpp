#include "../util/template.cpp"
#include "graph.cpp"

template <typename G>
class StronglyConnectedComponents {
    const G &graph;
    G rgraph;
    V<ll> label;
    V<ll> result;

    void dfs1(ll cur, ll &l) {
        label[cur] = l++;
        for (const G::Edge &e : graph[cur]) {
            ll nxt;
            tie(nxt, ignore) = e;
            if (label[nxt] != -1) continue;
            dfs1(nxt, l);
        }
    }

    void write_label() {
        ll l = 0;
        for (ll i = 0; i < graph.size(); i++) if (label[i] == -1) dfs1(i, l);
    }

    void dfs2(ll cur, ll l) {
        result[cur] = l;
        for (const G::Edge &e : rgraph[cur]) {
            ll nxt;
            tie(nxt, ignore) = e;
            if (result[nxt] != -1) continue;
            dfs2(nxt, l);
        }
    }

    void build_scc() {
        ll l = 0;
        for (ll i = (ll)graph.size() - 1; 0 <= i; i--) if (label[i] == -1) dfs2(i, l++);
    }

public:
    StronglyConnectedComponents(const G &graph)
        : graph(graph), label(graph.size(), -1), result(graph.size(), -1) 
    {
        rgraph = graph.build_inv();
    }

    V<ll> build() {
        write_label();
        build_scc();
        return result;
    }
};

using SCC = StronglyConnectedComponents<Graph<true>>;
