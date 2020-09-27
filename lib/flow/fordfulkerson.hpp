#pragma once
#include "util/template.hpp"
#include "base.hpp"

namespace flow {

template <typename Graph = FlowGraph<true>>
class FordFulkerson : public FlowSolverInterface<FordFulkerson, Graph> {
    Graph graph_;
    vec<bool> used;
    constexpr static Capacity cap0 = Capacity();

    Capacity dfs(Node cur, Node sink, Capacity f) noexcept {
        if (cur == sink) return f;
        used[cur] = true;
        for (auto &&e : graph_[cur]) {
            auto dst = e.dst;
            auto &cap = e.cap;
            if (used[dst]) continue;
            if (cap <= cap0) continue;
            auto rec = dfs(dst, sink, std::min(f, cap));
            if (cap0 < rec) {
                cap -= rec;
                graph_[dst][e.rev_idx].cap += rec;
                return rec;
            }
        }
        return cap0;
    }

public:
    FordFulkerson(Graph graph_arg) : graph_(std::move(graph_arg)), used(graph_.size()) { }

    Capacity solve(Node src, Node sink, Capacity limit) noexcept {
        auto ret = cap0;
        while (true) {
            std::fill(ALL(used), false);
            auto rec = dfs(src, sink, limit);
            if (rec == cap0) break;
            ret += rec;
        }
        return ret;
    }

    const Graph& graph() const noexcept {
        return graph_;
    }
};

}
