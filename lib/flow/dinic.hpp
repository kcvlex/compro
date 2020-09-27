#pragma once
#include "util/template.hpp"
#include "base.hpp"

namespace flow {

template <typename Graph = FlowGraph<true>>
class Dinic : public FlowSolverInterface<Dinic, Graph> {
    Graph graph_;
    vec<size_type> dists, idx_v;
    constexpr static size_type dinf = std::numeric_limits<size_type>::max() / 2;
    constexpr static Capacity cap0 = Capacity();

    void bfs(Node start) noexcept {
        std::fill(ALL(dists), dinf);
        dists[start] = 0;
        std::queue<std::pair<size_type, Node>> que;
        que.emplace(0, start);
        while (que.size()) {
            auto [ d, cur ] = que.front();
            que.pop();
            for (auto &&e : graph_[cur]) {
                auto [ nxt, cap, rev_idx, weight ] = e;
                if (cap <= cap0) continue;
                if (dists[nxt] <= d + 1) continue;
                dists[nxt] = d + 1;
                que.emplace(d + 1, nxt);
            }
        }
    }

    Capacity dfs(Node cur, Node pre, Node sink, Capacity f) noexcept {
        if (cur == sink) return f;
        for (auto &idx = idx_v[cur]; idx < size_type(graph_[cur].size()); idx++) {
            auto &[ to, cap, rev_idx, weight ] = graph_[cur][idx];
            if (dists[to] <= dists[cur]) continue;
            if (to == pre) continue;
            if (cap <= cap0) continue;
            auto rec = dfs(to, cur, sink, std::min(f, cap));
            cap -= rec;
            graph_[to][rev_idx].cap += rec;
            if (cap0 < rec) return rec;
        }
        return cap0;
    }

public:
    Dinic(Graph graph_arg) 
        : graph_(std::move(graph_arg)), 
          dists(graph_.size()),
          idx_v(graph_.size()) 
    {
    }

    Capacity flow(Node src, Node sink, Capacity limit) noexcept {
        Capacity ret = cap0;
        while (true) {
            bfs(src);
            if (dists[sink] == dinf) break;
            std::fill(ALL(idx_v), 0);
            while (true) {
                auto rec = dfs(src, -1, sink, limit);
                if (rec == cap0) break;
                ret += cap0;
            }
        }
        return ret;
    }

    const Graph& graph() const noexcept {
        return graph_;
    }
};

}
