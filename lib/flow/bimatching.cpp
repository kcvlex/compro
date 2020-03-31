#pragma once
#include "base.cpp"
#include "../graph/flow-graph.cpp"

namespace flow {

template <template <typename> typename Solver, 
          typename FlowGraph = graph::FlowGraph<true>>
class BipartiteMatching {
    ssize_t asz, bsz;
    FlowGraph fg;
    Node src, sink;

public:
    BipartiteMatching(ssize_t asz, ssize_t bsz)
        : asz(asz), bsz(bsz), fg(asz + bsz + 2), src(asz + bsz), sink(asz + bsz + 1)
    {
        for (ll i = 0; i < asz; i++) fg.add_edge(src, i, 1);
        for (ll i = 0; i < bsz; i++) fg.add_edge(i + asz, sink, 1);
    }

    void add_edge(Node f, Node t) {
        fg.add_edge(f, t + asz, 1);
    }

    ll solve() {
        Solver<FlowGraph> solver(fg);
        return solver.max_flow(src, sink);
    }
};

};
