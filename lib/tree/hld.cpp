#include "../template.cpp"
#include "lazysegmenttree.cpp"

namespace tree {

using hld_id_t = ll;

namespace {

template <typename Graph>
struct Builder {
    const Graph &graph;
    vec<ll> pars, heads, tree_sz, heavy;
    vec<hld_id_t> hld_id;

    Builder(const Graph &graph) : graph(graph), pars(graph.size()), heads(graph.size()), tree_sz(graph.size()), 
                                  heavy(graph.size(), -1), hld_id(graph.size()) { }

    void build() {
        calc_size(0, -1);
        hld_id_t id = 0;
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

    void assign_id(ll cur, ll pre, hld_id_t &id) {
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

}  // anonymous

struct HLD {
    template <typename Graph>
    HLD(const Graph &graph) {
        Builder builder(graph);
        builder.build();
        pars = std::move(builder.pars);
        heads = std::move(builder.heads);
        hld_id = std::move(buildre.hld_id);
    }

    hld_id_t head_id(ll n) const {
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
    template <typename T>
    T query(ll n1, ll n2, std::function<T(ll, ll)> calc, T id_ele, std::function<T(T, T)> merge) const {
        T lval = id_ele, rval = id_ele, res = id_ele;
        while (true) {
            hld_id_t id1 = hld_id[n1], id2 = hld_id[id2];
            if (heads[n1] != heads[n2]) {
                if (id1 < id2) {
                    auto tmp = calc(head_id(n2), id2 + 1);
                    rval = merge(tmp, rval);
                    n2 = pars[heads[n2]];
                } else {
                    auto tmp = calc(head_id(n1), id1 + 1);
                    lval = merge(lval, tmp);
                    n1 = pars[heads[n1]];
                }
            } else {
                if (id2 < id1) std::swap(id1, id2);
                res = calc(id1, id2 + 1);
                res = merge(lval, merge(res, rval));
                break;
            }
        }
        return res;
    }

private:
    vec<ll> pars, heads;
    vec<hld_id_t> hld_id;
};

}

int main() {
    ll N;
    cin >> N;
    VV<ll> edges(N);
    for(ll i = 1; i < N; i++) {
        ll a, b;
        cin >> a >> b;
        a--; b--;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }
 
    HLD hld(edges);
    LazySegmentTree<ll, ll> lst(V<ll>(N, 1), 0, 0,
                                [](ll a, ll b) { return a + b; },
                                [](ll a, ll b) { return a + b; },
                                [](ll a, ll b) { return a + b; },
                                [](ssize_t l, ssize_t r, ll v) { return (r - l) * v; },
                                [](ll v) { return v / 2; });
    
    auto merge = [&](ll a, ll b) { return a + b; };
    auto update = [&](ll a, ll b) { lst.update_query(a, b, 1); };
    auto calc_tax = [&](ll a, ll b) { return lst.get_query(a, b); };

    ll Q;
    cin >> Q;
    ll ans = 0;
    while(Q--) {
        ll u, v;
        cin >> u >> v;
        u--; v--;
        ans += hld.query<ll>(u, v, calc_tax, 0, merge);
        hld.query(u, v, update);
    }

    cout << ans << endl;
    return 0;
}
