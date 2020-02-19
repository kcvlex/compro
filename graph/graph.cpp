#include "../util/template.cpp"

template <bool Directed>
struct Graph : public VV<Edge> {
    using Node = ll;
    using Weight = ll;
    using Edge = pair<Node, Weight>;
    using VV<Edge>::VV;

    void add_edge(Node f, Node t, Weight w = 1) {
        (*this)[f].emplace_back(t, w);
        if (Directed) (*this)[t].emplace_back(f, w);
    }

    Graph<Directed> build_inv() const {
        Graph ret(this->size());
        for (Node i = 0; i < this->size(); i++) {
            for (const Edge &e : (*this)[i]) {
                Node j;
                Weight w;
                tie(j, w) = e;
                if (!Directed && j < i) continue;
                ret.add_edge(i, j, w);
            }
        }

        return ret;
    }
};
