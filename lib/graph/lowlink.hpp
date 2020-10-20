#include "../util/template.hpp"
#include "graph.hpp"

namespace graph {

template <typename Graph>
class LowLink {
    const Graph &graph;
    const size_type gsz;
    vec<ll> low, ord;
    vec<bool> visited, is_articulation;
    ll dfs_cnt;

    void dfs(Node cur, Node pre) {
        visited[cur] = true;
        ord[cur] = dfs_cnt++;
        low[cur] = ord[cur];
        ll children = 0;
        bool art = false;
        for (auto &&e : graph[cur]) {
            ll nxt = e.first;
            if (!visited[nxt]) {
                children++;
                dfs(nxt, cur);
                chmin(low[cur], low[nxt]);
                art |= (pre == -1 ? 2 <= children : ord[cur] <= low[nxt]);
            } else if (nxt != pre) {
                chmin(low[cur], ord[nxt]);
            }
        }
        is_articulation[cur] = art;
    }

public:
    LowLink(const Graph &graph) 
        : graph(graph), gsz(graph.size()), low(gsz), ord(gsz),
          visited(gsz), is_articulation(gsz), dfs_cnt(0)
    {
        for (Node i = 0; i < gsz; i++) if (!visited[i]) dfs(i, -1);
    }
    
    bool is_art(Node cur) const {
        return is_articulation[cur]; 
    }

    // u -> v
    bool is_bridge(Node from, Node to) const {
        return ord[from] < low[to];
    }

    vec<pll> all_bridges() const {
        vec<pll> ret;
        for (Node from = 0; from < gsz; from++) for (auto &&e : edges[from]) {
            Node to = e.first;
            if (!Dir && !(from < to)) continue;
            if (is_bridge(from, to)) ret.emplace_back(from, to);
        }
        return ret;
    }
};

}
