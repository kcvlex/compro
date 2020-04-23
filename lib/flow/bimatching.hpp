#pragma once
#include "base.hpp"
#include "../graph/flow-graph.hpp"

namespace flow {

template <template <typename> typename Solver, 
          typename FlowGraph = graph::FlowGraph<true>,
          bool Restore = false>
class BipartiteMatching {
    using matching = std::pair<Node, Node>;
    ssize_t asz, bsz;
    FlowGraph fg, res;
    Node src, sink;
    std::set<std::tuple<Node, Node, ll>> edges;

    vec<matching> extract_() {
        vec<matching> ret;
        for (auto &&e : edges) {
            Node f, t;
            ll idx;
            std::tie(f, t, idx) = e;
            auto c = res[f][idx].cap();
            if (c == 0) ret.emplace_back(f, t - asz);
        }
        return ret;
    }

public:
    BipartiteMatching(ssize_t asz, ssize_t bsz)
        : asz(asz), bsz(bsz), fg(asz + bsz + 2), src(asz + bsz), sink(asz + bsz + 1)
    {
        for (ll i = 0; i < asz; i++) fg.add_edge(src, i, 1);
        for (ll i = 0; i < bsz; i++) fg.add_edge(i + asz, sink, 1);
    }

    void add_edge(Node f, Node t) {
        fg.add_edge(f, t + asz, 1);
        if (Restore) edges.emplace(f, t + asz, fg[f].size() - 1);
    }

    ll solve() {
        Solver<FlowGraph> solver(fg);
        auto ret = solver.max_flow(src, sink);
        if (Restore) res = std::move(solver.graph());
        return ret;
    }

    vec<matching> extract() {
        if (Restore) {
            return extract_();
        } else {
            assert(false);
        }
    }

};

}
