#pragma once
#include "graph.hpp"

namespace graph {

// labels is assumed to be compressed
// unweighted graph is assumed
// self loop
template <typename Graph>
auto compose_graph(const Graph &g, const vec<ll> &labels, bool self_loop) -> std::pair<Graph, vvec<ll>> {
    ll maxv = *std::max_element(ALL(labels));
    Graph ret(maxv + 1);
    vvec<ll> nodes(maxv + 1);
    for (Node cur = 0; cur < g.size(); cur++) {
        ll l = labels[cur];
        nodes[l].push_back(cur);
        for (Node nxt : graph::dst(g[i])) {
            ll nl = labels[nxt];
            if (nl != l || self_loop) ret.add_edge(l, nl);
        }
    }
    for (auto &lis : ret) {
        std::sort(ALL(lis));
        lis.erase(std::unique(ALL(lis)), lis.end());
    }
    return std::make_pair(std::move(ret), std::move(nodes));
}

}
