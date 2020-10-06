#pragma once
#include "util/template.hpp"

namespace flow {

using Node = ll;
using Capacity = ll;
using Weight = ll;

struct FlowEdge {
    Node dst;
    Capacity cap;
    size_type rev_idx;
    Weight weight;

    FlowEdge(Node dst, Capacity cap, size_type rev_idx, Weight weight)
        : dst(dst), cap(cap), rev_idx(rev_idx), weight(weight) { }

    FlowEdge() : FlowEdge(Node(), Capacity(), size_type(), Weight()) { }
};

template <bool Directed>
struct FlowGraph : public vvec<FlowEdge> {
    using vvec<FlowEdge>::vvec;

    void add_edge(Node f, Node t, Capacity c = 1, Weight w = 1) {
        add_edge_with_rev(f, t, c, w);
        if (!Directed) add_edge_with_rev(t, f, c, w);
    }

private:
    void add_edge_with_rev(Node f, Node t, Capacity c, Weight w) {
        FlowEdge fe(t, c, (ll)((*this)[t].size()), w);
        (*this)[f].push_back(fe);
        FlowEdge rfe(f, Capacity(), (ll)((*this)[f].size()) - 1, -w);
        (*this)[t].push_back(rfe);
    }
};

template <template <typename> typename FlowSolverImpl,
          typename Graph>
struct FlowSolverInterface {
    using solver_impl_type = FlowSolverImpl<Graph>;

    Capacity flow(Node src, Node sink, Capacity limit) noexcept {
        return static_cast<FlowSolverImpl<Graph>*>(this)->flow(src, sink, limit);
    }

    Capacity flow(Node src, Node sink) noexcept {
        return flow(src, sink, std::numeric_limits<Capacity>::max() / 10);
    }

    const Graph& graph() const noexcept {
        return static_cast<const FlowSolverImpl<Graph>*>(this)->graph();
    }
};

class NodeGenerator {
    size_type offset, sz;

public:
    NodeGenerator(size_type offset, size_type sz) : offset(offset), sz(sz) { }
    
    struct Node_ {
        Node v;
        Node_(Node v) : v(v) { };
    };

    Node_ operator()(size_type idx) const {
        assert(0 <= idx && idx < sz);
        return Node_(idx + offset);
    }
};

}
