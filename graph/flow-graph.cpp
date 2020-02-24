#pragma once
#include "graph.cpp"

namespace graph {

using Capacity = ll;

struct FlowEdge : public std::tuple<Node, Capacity, ll> {
    using std::tuple<Node, Capacity, ll>::tuple;

    Node& to() {
        return get<0>(*this);
    }

    const Node& to() const {
        return get<0>(*this);
    }

    Capacity& cap() {
        return get<1>(*this);
    }

    const Capacity& cap() const {
        return get<1>(*this);
    }

    ll& rev_idx() {
        return get<2>(*this);
    }

    const ll& rev_idx() const {
        return get<2>(*this);
    }
};

template <bool Directed>
struct FlowGraph : public vvec<FlowEdge> {
    using vvec<FlowEdge>::vvec;

    void add_edge(Node f, Node t, Capacity c = 1) {
        add_edge_with_rev(f, t, c);
        if (!Directed) add_edge_with_rev(t, f, c);
    }

private:
    void add_edge_with_rev(Node f, Node t, Capacity c) {
        FlowEdge fe(t, c, (ll)((*this)[t].size()));
        (*this)[f].push_back(fe);
        FlowEdge rfe(f, Capacity(), (ll)((*this)[f].size()) - 1);
        (*this)[t].push_back(rfe);
    }
};

}
