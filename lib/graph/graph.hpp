#pragma once
#include "../util/template.hpp"

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

template <typename Iterator>
class dst_iterator {
    Iterator ite;

public:
    dst_iterator(Iterator ite) : ite(ite) { }

    bool operator ==(const dst_iterator<Iterator> &oth) const {
        return ite == oth.ite;
    }

    bool operator !=(const dst_iterator<Iterator> &oth) const {
        return !(*this == oth);
    }

    bool operator <(const dst_iterator<Iterator> &oth) const {
        return ite < oth.ite;
    }

    bool operator >(const dst_iterator<Iterator> &oth) const {
        return ite > oth.ite;
    }

    bool operator <=(const dst_iterator<Iterator> &oth) const {
        return ite <= oth.ite;
    }

    bool operator >=(const dst_iterator<Iterator> &oth) const {
        return ite >= oth.ite;
    }

    const Node& operator *() {
        return ite->first;
    }

    const Node& operator *() const {
        return ite->first;
    }

    dst_iterator operator ++() {
        ++ite;
        return ite;
    }
};

class dst_iteration {
    using ite_type = vec<Edge>::const_iterator;
    const vec<Edge> &edges;

public:
    dst_iteration(const vec<Edge> &edges) : edges(edges) { }

    auto begin() const {
        return dst_iterator<ite_type>(edges.cbegin());
    }

    auto end() const {
        return dst_iterator<ite_type>(edges.cend());
    }
};

class dst_reverse_iteration {
    using ite_type = vec<Edge>::const_reverse_iterator;
    const vec<Edge> &edges;

public:
    dst_reverse_iteration(const vec<Edge> &edges) : edges(edges) { }

    auto begin() const {
        return dst_iterator<ite_type>(edges.crbegin());
    }

    auto end() const {
        return dst_iterator<ite_type>(edges.crend());
    }
};

dst_iteration dst(const vec<Edge> &edges) {
    return dst_iteration(edges);
}

dst_reverse_iteration rdst(const vec<Edge> &edges) {
    return dst_reverse_iteration(edges);
}

}
