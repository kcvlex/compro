#include "../util/template.cpp"
#include "graph.cpp"

namespace graph {

template <typename Graph>
struct topological_sort {
    const Graph &g;
    ssize_t n;
    vec<Node> ret;
    vec<bool> pass, used;

    topological_sort(const Graph &g) : g(g), n(g.size()), pass(n), used(n)
    {
    }

    bool dfs(ll cur) {
        used[cur] = true;
        pass[cur] = true;
        for (auto &&nxt : dst(g[cur])) {
            if (pass[nxt]) return false;
            if (used[nxt]) continue;
            if (!dfs(nxt)) return false;
        }
        ret.push_back(cur);
        pass[cur] = false;
        return true;
    }

    vec<Node> solve() {
        for (Node i = 0; i < n; i++) {
            if (used[i]) continue;
            if (!dfs(i)) {
                ret.clear();
                break;
            }
        }
        std::reverse(ALL(ret));
        return ret;
    }
};

template <typename Graph>
vec<Node> topsort(const Graph &g) {
    return topological_sort<Graph>(g).solve();
}

}
