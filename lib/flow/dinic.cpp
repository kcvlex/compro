#pragma once
#include "../util/template.cpp"
#include "../graph/flow-graph.cpp"
#include "base.cpp"

namespace flow {

template <typename FlowGraph = graph::FlowGraph<true>>
class Dinic {
    FlowGraph flow_graph;
    vec<ll> dists;
    vec<ssize_t> used;
    Node src, sink;

    void bfs(Node start) {
        vec<ll> dists(flow_graph.size(), dinf);
        dists[start] = 0;
        std::queue<std::pair<ll, Node>> que;
        que.emplace(0, start);
        while (que.size()) {
            ll d;
            Node cur;
            std::tie(d, cur) = que.front();
            que.pop();
            for (const auto &e : flow_graph[cur]) {
                Node nxt;
                Capacity cap;
                tie(nxt, cap, std::ignore, std::ignore) = e;
                if (cap <= Capacity()) continue;
                if (dists[nxt] <= d + 1) continue;
                dists[nxt] = d + 1;
                que.emplace(d + 1, nxt);
            }
        }
        this->dists = move(dists);
    }

    Capacity dfs(Node cur, Node pre, Capacity flow) {
        if (cur == sink) return flow;
        for (auto idx = used[cur] + 1; idx < flow_graph[cur].size(); idx++) {
            used[cur]++;
            auto &e = flow_graph[cur][idx];
            if (dists[e.to()] <= dists[cur]) continue;
            if (e.cap() <= Capacity()) continue;
            if (e.to() == pre) continue;
            auto f = dfs(e.to(), cur, std::min(flow, e.cap()));
            e.cap() -= f;
            flow_graph[e.to()][e.rev_idx()].cap() += f;
            if (Capacity() < f) return f;
        }
        return Capacity();
    }

public:
    Dinic(const FlowGraph &flow_graph) :
        flow_graph(flow_graph), used(flow_graph.size()) { }

    Capacity max_flow(Node src, Node sink) {
        this->src = src;
        this->sink = sink;
        auto ret = Capacity();
        while (true) {
            bfs(src);
            if (dists[sink] == dinf) break;
            std::fill(ALL(used), -1);
            while (true) {
                auto tmp = dfs(src, -1, cinf);
                if (tmp == Capacity()) break;
                ret += tmp;
            }
        }
        return ret;
    }
};

}
