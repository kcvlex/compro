#pragma once
#include "../util/template.cpp"

namespace graph {

using Node = ll;
using Weight = ll;
using Edge = std::pair<Node, Weight>;

template <bool Directed>
struct Graph : public vvec<Edge> {
    using vvec<Edge>::vvec;

    void add_edge(Node f, Node t, Weight w = 1) {
        (*this)[f].emplace_back(t, w);
        if (!Directed) (*this)[t].emplace_back(f, w);
    }

    Graph<Directed> build_inv() const {
        Graph<Directed> ret(this->size());
        for (Node i = 0; i < this->size(); i++) {
            for (const Edge &e : (*this)[i]) {
                Node j;
                Weight w;
                std::tie(j, w) = e;
                if (!Directed && j < i) continue;
                ret.add_edge(j, i, w);
            }
        }

        return ret;
    }
};

}
