#include "../template.hpp"
#include "lazysegtree.hpp"

namespace tree {

using hld_id_type = ssize_t;

namespace builder {

template <typename Graph>
struct Builder {
    const Graph &graph;
    vec<ll> pars, heads, tree_sz, heavy;
    vec<hld_id_t> hld_id;

    Builder(const Graph &graph) : graph(graph), pars(graph.size()), heads(graph.size()), tree_sz(graph.size()), 
                                  heavy(graph.size(), -1), hld_id(graph.size()) { }

    void build() {
        calc_size(0, -1);
        hld_id_type id = 0;
        assign_id(0, -1, id);
    }

private:
    ll calc_sz(ll cur, ll pre) {
        ll ret = 1;
        pars[cur] = pre;
        for (auto &&e : graph[cur]) {
            ll nxt;
            std::tie(nxt, std::ignore) = e;
            if (nxt == pre) continue;
            ret += calc_sz(nxt, cur);
            bool is_heavy = (heavy[cur] == -1 || tree_sz[heavy[cur]] < tree_sz[nxt]);
            if (is_heavy) heavy[cur] = nxt;
        }
        return tree_sz[cur] = ret;
    }

    void assign_id(ll cur, ll pre, hld_id_type &id) {
        hld_id[cur] = id++;
        if (pre == -1) heads[cur] = cur;
        else heads[cur] = (heavy[pre] == cur ? heads[pre] : cur);
        if (heavy[cur] != -1) assign_id(heavy[cur], cur, id);  // FIXME : heavy[cur] != cur is necessary ??
        for (auto &&e : graph[cur]) {
            ll nxt;
            std::tie(nxt, std::ignore) = e;
            if (nxt == pre) continue;
            if (nxt == heavy[cur]) continue;
            assign_id(nxt, cur, id);
        }
    }
};

}

struct HLD {
    template <typename Graph>
    HLD(const Graph &graph) {
        builder::Builder builder(graph);
        builder.build();
        pars = std::move(builder.pars);
        heads = std::move(builder.heads);
        hld_id = std::move(buildre.hld_id);
    }

    hld_id_type head_id(ll n) const {
        return hld_id[heads[n]];
    }

    ll lca(ll n1, ll n2) const {
        while (true) {
            if (hld_id[n2] < hld_id[n1]) std::swap(n1, n2);
            if (heads[n1] == heads[n2]) break;
            n2 = pars[heads[n2]];
        }
        return n1;
    }

    // calc's arg is [l, r)
    template <typename T, typename F>
    Monoid<F> query(ll n1, ll n2, F f) const {
        Monoid<F> res = Monoid::id_ele();
        Monoid<F> lval = res, rval = res;
        while (true) {
            hld_id_type id1 = hld_id[n1], id2 = hld_id[id2];
            if (heads[n1] != heads[n2]) {
                if (id1 < id2) {
                    Monoid<F> add = f(head_id(n2), id2 + 1);
                    rval = Monoid<F>::merge(add, rval);
                    n2 = pars[heads[n2]];
                } else {
                    Monoid<F> add = f(head_id(n1), id1 + 1);
                    lval = Monoid<F>::merge(lval, add);
                    n1 = pars[heads[n1]];
                }
            } else {
                if (id2 < id1) std::swap(id1, id2);
                res = calc(id1, id2 + 1);
                res = Monoid<F>::merge(lval, res);
                res = Monoid<F>::merge(res, rval);
                break;
            }
        }
        return res;
    }

private:
    template <typename F>
    using Monoid = typename std::invoke_result<F, hld_id_type, hld_id_type>::type;
    
    vec<ll> pars, heads;
    vec<hld_id_t> hld_id;
};

}
