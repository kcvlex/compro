#pragma once
#include "graph.hpp"

namespace graph {

using Capacity = ll;

struct FlowEdge : public std::tuple<Node, Capacity, ll, Weight> {
    using std::tuple<Node, Capacity, ll, Weight>::tuple;

    Node& to() {
        return std::get<0>(*this);
    }

    const Node& to() const {
        return std::get<0>(*this);
    }

    Capacity& cap() {
        return std::get<1>(*this);
    }

    const Capacity& cap() const {
        return std::get<1>(*this);
    }

    ll& rev_idx() {
        return std::get<2>(*this);
    }

    const ll& rev_idx() const {
        return std::get<2>(*this);
    }

    Weight& weight() {
        return std::get<3>(*this);
    }

    const Weight& weight() const {
        return std::get<3>(*this);
    }
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

}
