#pragma once
#include "../template.cpp"
#include "../graph/graph.cpp"

namespace tree {

template <typename Graph>
class TreeCentroid {
    const Graph &g;
    const ssize_t n;
    vec<bool> used;
    vec<ssize_t> sub_sz;
    vvec<ll> cen_ch;
    Node root;

    ssize_t dfs(Node cur, Node pre) {
        sub_sz[cur] = 1;
        for (auto &&nxt : graph::dst(g[cur])) {
            if (used[nxt]) continue;
            if (nxt == pre) continue;
            sub_sz[cur] += dfs(nxt, cur);
        }
        return sub_sz[cur];
    }

    Node search_cen(Node cur, Node pre, ssize_t tree_sz) {
        if (pre == -1) tree_sz = dfs(cur, pre);
        
        std::pair<ssize_t, Node> large_ch(-1, -1);
        for (ll ch : graph::dst(g[cur])) {
            if (used[ch]) continue;
            if (ch == pre) continue;
            chmax(large_ch, pll(sub_sz[ch], ch));
        }

        ssize_t lsz;
        Node lch;
        std::tie(lsz, lch) = large_ch;

        if (lsz < tree_sz / 2) {
            used[cur] = true;
            if (root == -1) root = cur;
            for (Node ch : graph::dst(g[cur])) {
                if (used[ch]) continue;
                Node croot = search(ch, -1, -1);
                cen_ch[cur].push_back(croot);
            }
        } else {
            return search(lch, cur, tree_sz);
        }
    }

public:
    TreeCentroid(const Graph &g) 
        : g(g), n(g.size()), used(n), sub_sz(n), cen_ch(n), root(-1)
    {
    }

    void solve() {
        search_cen(0, -1, n);
    }
};

template <typename Graph>
std::pair<Node, vvec<Node>> tree_centroid(const Graph &tree) {
    TreeCentroid tc(tree);
    tc.solve();
    return std::make_pair(tc.root, std::move(tc.cen_ch));
}

}
