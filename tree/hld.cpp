#include "../template.cpp"
#include "lazysegmenttree.cpp"

struct HLD {
    V<ll> p_node, head_node, hld_id, heavy , tree_size;
    HLD(const VV<ll> &edges) : p_node(edges.size()), head_node(edges.size()),
                               hld_id(edges.size()), heavy(edges.size(), -1), 
                               tree_size(edges.size())
    {
        calc_size(0, -1, edges);
        ll id = 0;
        calc_id(0, -1, edges, id);
    }

    ll calc_size(ll cur, ll pre, const VV<ll> &edges) {
        ll ret = 1;
        p_node[cur] = pre;
        for(ll nxt : edges[cur]) if(nxt != pre) {
            ret += calc_size(nxt, cur, edges);
            if(heavy[cur] == -1 || 
               tree_size[heavy[cur]] < tree_size[nxt]) {
                heavy[cur] = nxt;
            }
        }
        return tree_size[cur] = ret;
    }

    void calc_id(ll cur, ll pre, const VV<ll> &edges, ll &id) {
        if(cur == -1) return;
        hld_id[cur] = id++;
        if(pre == -1) head_node[cur] = cur;
        else head_node[cur] = (heavy[pre] == cur ? head_node[pre] : cur);
        if(cur != heavy[cur]) calc_id(heavy[cur], cur, edges, id);
        for(ll nxt : edges[cur]) {
            if(nxt == pre || nxt == heavy[cur]) continue;
            calc_id(nxt, cur, edges, id);
        }
    }

    ll head_id(ll node) { return hld_id[head_node[node]]; }

    ll lca(ll n1, ll n2) {
        while(true) {
            if(hld_id[n2] < hld_id[n1]) swap(n1, n2);
            if(head_node[n1] == head_node[n2]) break;
            n2 = p_node[head_node[n2]];
        }
        return n1;
    }

    // calc's arg is [l, r)
    template <typename T>
    T query(ll n1, ll n2, function<T(ll, ll)> calc, 
            T id_ele, function<T(T, T)> merge) 
    {
        T lval = id_ele, rval = id_ele;
        T res = id_ele;
        while(true) {
            ll id1 = hld_id[n1];
            ll id2 = hld_id[n2];
            if(head_node[n1] != head_node[n2]) {
                if(id1 < id2) {
                    auto tmp = calc(head_id(n2), id2 + 1);
                    rval = merge(tmp, rval);
                    n2 = p_node[head_node[n2]];
                } else {
                    auto tmp = calc(head_id(n1), id1 + 1);
                    lval = merge(lval, tmp);
                    n1 = p_node[head_node[n1]];
                }
            } else {
                if(id2 < id1) swap(id1, id2);
                res = calc(id1, id2 + 1);
                res = merge(lval, merge(res, rval));
                break;
            }
        }
        return res;
    }

    void query(ll n1, ll n2, function<void(ll, ll)> update) {
        ll identity = 0;
        auto merge = [&](ll a, ll b) { return 0; };
        auto wrapper_calc = [&](ll a, ll b) { update(a, b); return 0; };
        query<ll>(n1, n2, wrapper_calc, identity, merge);
    }
};

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
