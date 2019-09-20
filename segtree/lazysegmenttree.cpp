#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;

template <typename T, typename L>
class LazySegmentTree {
private:
    using Merge = function<T(T, T)>;
    using Apply = function<T(T, L)>;
    using Update = function<L(L, L)>;
    using CalcLazyValue = function<L(ssize_t, ssize_t, L)>;
    using Prop = function<L(L)>;

    size_t arr_size;
    T init_node;
    L init_lazy;
    vector<T> node;
    vector<L> lazy;
    vector<bool> lazy_flag;
    Merge merge_node;
    Apply apply_lazy_value;
    Update update_lazy_value;
    CalcLazyValue calc_lazy_value;
    Prop prop_lazy_value;
    
public:
    LazySegmentTree(vector<T> v, 
                    T init_node,
                    L init_lazy, 
                    Merge merge_node,
                    Apply apply_lazy_value,
                    Update update_lazy_value,
                    CalcLazyValue calc_lazy_value,
                    Prop prop_lazy_value = [](L l) { return l; })
        : init_node(init_node),
          init_lazy(init_lazy),
          merge_node(merge_node),
          apply_lazy_value(apply_lazy_value),
          update_lazy_value(update_lazy_value),
          calc_lazy_value(calc_lazy_value),
          prop_lazy_value(prop_lazy_value)
    {
        {
            arr_size = 1;
            while(arr_size < v.size()) arr_size *= 2;
        }
        node.resize(2 * arr_size - 1, init_node);
        lazy.resize(2 * arr_size - 1, init_lazy);
        lazy_flag.resize(2 * arr_size - 1, false);
        for(ll i = 0; i < v.size(); i++) node[i + arr_size - 1] = v[i];
        for(ll i = arr_size - 2; 0 <= i; i--) node[i] = merge_node(node[i * 2 + 1], node[i * 2 + 2]);
    }

    void lazy_eval(ll pos, ll left, ll right) {
        if(!lazy_flag[pos]) return;

        node[pos] = apply_lazy_value(node[pos], lazy[pos]);
        lazy_flag[pos] = false;

        if(right - left > 1) {
            for(ll idx = 2 * pos + 1; idx <= 2 * pos + 2; idx++) {
                lazy[idx] = update_lazy_value(lazy[idx], prop_lazy_value(lazy[pos]));
                lazy_flag[idx] = true;
            }
        }

        lazy[pos] = init_lazy;
    }

    void update_query(ll left, ll right, L val, ll pos = 0, ll node_left = 0, ll node_right = -1) {
        if(node_right < 0) node_right = arr_size;

        lazy_eval(pos, node_left, node_right);

        if(right <= node_left || node_right <= left) return;
        
        if(left <= node_left && node_right <= right) {
            lazy[pos] = calc_lazy_value(node_left, node_right, val);
            lazy_flag[pos] = true;
            lazy_eval(pos, node_left, node_right);
        } else {
            ll mid = (node_left + node_right) / 2;
            update_query(left, right, val, 2 * pos + 1, node_left, mid);
            update_query(left, right, val, 2 * pos + 2, mid, node_right);
            node[pos] = merge_node(node[2 * pos + 1], node[2 * pos + 2]);
        }
    }

    T get_query(ll left, ll right, ll pos = 0, ll node_left = 0, ll node_right = -1) {
        if(node_right < 0) node_right = arr_size;
        
        lazy_eval(pos, node_left, node_right);

        if(node_right <= left || right <= node_left) return init_node;
        if(left <= node_left && node_right <= right) return node[pos];

        ll mid = (node_left + node_right) / 2;
        return merge_node(get_query(left, right, 2 * pos + 1, node_left, mid),
                          get_query(left, right, 2 * pos + 2, mid, node_right));
    }
};

// solution for http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F

#ifdef VERIFY
int main() {
    ll N;
    cin >> N;
    ll init = (1ll << 31) - 1;
    LazySegmentTree<ll, ll> LST(
            vector<ll>(N, init), init, init,
            [](ll n1, ll n2) { return min(n1, n2); },
            [](ll n, ll l) { return l; },
            [](ll lc, ll lp) { return lp; },
            [](ll l, ll r, ll v) { return v; });
    ll Q;
    cin >> Q;
    for(ll QQ = 0; QQ < Q; QQ++) {
        ll qn;
        cin >> qn;
        if(qn == 0) {
            ll s, t, x;
            cin >> s >> t >> x;
            LST.update_query(s, t + 1, x);
        } else {
            ll s, t;
            cin >> s >> t;
            cout << LST.get_query(s, t + 1) << '\n';
        }
    }
    return 0;
}
#endif
