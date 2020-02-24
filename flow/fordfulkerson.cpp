#pragma once

#include "../util/template.cpp"
#include "../graph/graph.cpp"
#include "../graph/flow-graph.cpp"

namespace flow {
    
using graph::Node;
using graph::Capacity;

template <typename FlowGraph>
class FordFulkerson {
    FlowGraph flow_graph;
    const graph::Capacity inf;
    vec<bool> used;
    Node src, sink;

    Capacity dfs(Node cur, Capacity f) {
        if (cur == sink) return f;
        used[cur] = true;
        for (auto &&e : flow_graph[cur]) {
            if (used[e.to()]) continue;
            if (e.cap() <= Capacity()) continue;
            auto rec = dfs(e.to(), min(f, e.cap()));
            if (Capacity() < rec) {
                e.cap() -= rec;
                flow_graph[e.to()][e.rev_idx()].cap() += rec;
                return rec;
            }
        }
        return 0;
    }

public:
    FordFulkerson(FlowGraph flow_graph, const Capacity &inf) :
        flow_graph(flow_graph), inf(inf), used(flow_graph.size()) { }

    Capacity max_flow(Node src, Node sink) {
        this->src = src;
        this->sink = sink;
        auto ret = Capacity();
        while (true) {
            std::fill(ALL(used), false);
            auto tmp = dfs(src, inf);
            if (tmp == Capacity()) break;
            ret += tmp;
        }
        return ret;
    }
};

}
