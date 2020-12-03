#pragma once
#include "util/template.hpp"

namespace tree {

struct LCA {
    using node_type = int;
    
    const size_type sz;
    const node_type root;

    template <typename Graph>
    LCA(const Graph &g, node_type root_arg)
        : sz(g.size()), root(root_arg), depth(sz), par(sz)
    {
        dfs(root, dummy, 0, g);
        std::fill(ALL(par[root]), dummy);
        for (int d = 0; d + 1 < log_sz; d++) for (int i = 0; i < sz; i++) {
            int p = par[i][d];
            par[i][d + 1] = (p == dummy ? dummy : par[p][d]);
        }
    }

    int get_depth(node_type i) const {
        return depth[i];
    }

    node_type operator()(node_type a, node_type b) const {
        if (depth[a] < depth[b]) std::swap(a, b);
        a = get_parent(a, depth[a] - depth[b]);
        for (int d = log_sz - 1; 0 <= d; d--) {
            auto p1 = get_parent(a, d), p2 = get_parent(b, d);
            if (p1 != p2) {
                a = p1;
                b = p2;
            }
        }
        return par[a][0];
    }

private:
    static constexpr size_type log_sz = 30;
    static constexpr node_type dummy = -1;
    vec<int> depth;
    vec<std::array<node_type, log_sz>> par;

    node_type get_parent(node_type a, int d) const {
        for (int i = 0; d; i++, d /= 2) {
            if (a == dummy) return dummy;
            if (d & 1) a = par[a][d];
        }
        return a;
    }
};

}
