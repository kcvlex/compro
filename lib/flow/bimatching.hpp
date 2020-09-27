#pragma once
#include "base.hpp"

namespace flow {

template <template <typename> typename FlowSolverImpl,
          typename Graph = FlowGraph<true>>
class BipartiteMatching {
    using matching = std::pair<Node, Node>;
    using solver_type = FlowSolverInterface<FlowSolverImpl, Graph>;
    using solver_impl_type = FlowSolverImpl<Graph>;

    size_type asz, bsz;
    Graph fg;
    Node src, sink;
    std::unique_ptr<solver_type> solver;

    BipartiteMatching(size_type asz, size_type bsz)
        : asz(asz), 
          bsz(bsz), 
          fg(asz + bsz + 2),
          src(asz + bsz), 
          sink(src + 1),
          solver(nullptr)
    {
        for (size_type i = 0; i < asz; i++) fg.add_edge(src, i, 1);
        for (size_type i = 0; i < bsz; i++) fg.add_edge(i + asz, sink, 1);
    }

    struct Node_ {
        Node v;
        Node_(Node v) : v(v) { };
    };

public:
    class NodeGenerator {
        size_type offset, sz;

    public:
        NodeGenerator(size_type offset, size_type sz) : offset(offset), sz(sz) { }

        Node_ operator()(size_type idx) const {
            assert(0 <= idx && idx < sz);
            return Node_(idx + offset);
        }
    };

    static auto make(size_type asz, size_type bsz) {
        auto m = BipartiteMatching<FlowSolverImpl, Graph>(asz, bsz);
        auto gen1 = NodeGenerator(0, asz);
        auto gen2 = NodeGenerator(asz, bsz);
        return std::make_tuple(BipartiteMatching<FlowSolverImpl, Graph>(asz, bsz),
                               NodeGenerator(0, asz),
                               NodeGenerator(asz, bsz));
    }

    void add_edge(Node_ a, Node_ b) {
        fg.add_edge(a.v, b.v, 1);
    }

    Capacity solve() {
        solver = std::move(std::make_unique<solver_impl_type>(fg));
        return solver->flow(src, sink);
    }

    vec<matching> extract() const noexcept {
        vec<matching> ret;
        const auto &g = solver->graph();
        for (Node a = 0; a < asz; a++) for (const auto &e : g[a]) {
            auto [ b, cap, rev_idx, weight ] = e;
            if (!(0 <= b - asz && b - asz < bsz)) continue;
            if (cap == 0) {
                ret.emplace_back(a, b - asz);
                break;
            }
        }
        return ret;
    }

};

}
