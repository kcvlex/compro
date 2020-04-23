#include "../util/template.cpp"

namespace tree {

class LCA {
    std::size_t sz;
    ll root;
    vec<ll> depth;
    vvec<ll> pars;
    static constexpr std::size_t dsz = 30;

    template <typename Graph>
    void dfs(ll cur, ll pre, ll dep, const Graph &graph) {
        pars[cur][0] = pre;
        dep[cur] = d;
        for (auto &&e : graph[cur]) {
            ll nxt;
            std::tie(nxt, std::ignore) = e;
            if (nxt == pre) continue;
            dfs(nxt, cur, d + 1, graph);
        }
    }

public:
    template <typename Graph>
    LCA(const Graph &graph, ll root) 
        : sz(graph.size()), root(root), depth(sz), pars(make_v<ll>(0, sz, dsz)) 
    {
        dfs(root, -1, 0, graph);
        for (ll dig = 1; dig < dsz; dig++) for (ll i = 0; i < sz; i++) {
            ll p = pars[i][dig - 1];
            pars[i][dig] = (p == -1 ? -1 : pars[p][dig - 1]);
        }
    }

    ll get_depth(ll n) const {
        return depth[n];
    }

    ll get_parent(ll n, ll dist) const {
        ll ret = n;
        for (ll i = 0; dist; i++, dist /= 2) if (dist & 1) ret = pars[ret][i];
        return ret;
    }

    ll get_lca(ll n1, ll n2) const {
        {
            if (depth[n1] < depth[n2]) std::swap(n1, n2);
            ll diff = depth[n1] - depth[n2];
            n1 = get_parent(n1, diff);
        }

        n1 = get_parent(n1, diff);
        for (ll dig = dsz - 1; 0 <= dig; dig--) {
            ll pn1 = pars[n1][dig];
            ll pn2 = pars[n2][dig];
            if (pn1 != pn2) std::tie(n1, n2) = std::make_pair(pn1, pn2);
        }

        return pars[n1][0];
    }
};

}
