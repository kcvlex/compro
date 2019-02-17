#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;

template <typename T, typename L>
class LazySegmentTree{
private:
    ll N;
    T init_node;
    L init_lazy;
    vector<T> node;
    vector<L> lazy;
    vector<bool> lazy_flag;
    function<T (T, T)> merge_node;
    function<T (T, L)> apply_lazy_value;
    function<L (L, L)> update_lazy_value;
    function<L (ll, ll, L)> create_lazy_value;
    
public:
    LazySegmentTree(const vector<T> &v, 
                    const T &init_node,
                    const L &init_lazy, 
                    const decltype(merge_node)        &merge_node,
                    const decltype(apply_lazy_value)  &apply_lazy_value,
                    const decltype(update_lazy_value) &update_lazy_value,
                    const decltype(create_lazy_value) &create_lazy_value) 
        : init_node(init_node),
          init_lazy(init_lazy),
          merge_node(merge_node),
          apply_lazy_value(apply_lazy_value),
          update_lazy_value(update_lazy_value),
          create_lazy_value(create_lazy_value)
    {
        ll tmp = 1;
        while(tmp < v.size()) tmp *= 2;
        N = tmp;
        node.resize(2 * N - 1, init_node);
        lazy.resize(2 * N - 1, init_lazy);
        lazy_flag.resize(2 * N - 1, false);
        for(ll i = 0; i < v.size(); i++) {
            node[i + N - 1] = v[i];
        }
        for(ll i = N - 2; 0 <= i; i--) {
            node[i] = merge_node(node[i * 2 + 1], node[i * 2 + 2]);
        }
    }

    /*
     * node[pos] -> [left, right)
     */
    void lazy_eval(ll pos, ll left, ll right) {
        if(!lazy_flag[pos]) {
            return;
        }

        node[pos] = apply_lazy_value(node[pos], lazy[pos]);
        lazy_flag[pos] = false;

        /* 
         * whether the node is the bottom of tree or not.
         */
        if(right - left > 1) {
            for(ll idx : {2 * pos + 1, 2 * pos + 2}) {
                lazy[idx] = update_lazy_value(lazy[idx], lazy[pos]);
                lazy_flag[idx] = true;
            }
        }

        lazy[pos] = init_lazy;
    }

    /*
     * If you want to call this func from out of class, in many cases you don't have to change the args pos, node_left, node_right.
     * Be careful that the range is half-open interval.
     * [left, right), [node_left, node_right)
     * @param left:         lower limit of interval of query
     * @param right:        upper limit of interval of query
     * @param val:          the value gave from query
     * @param node_left:    lower limit of interval of the node points.
     * @param node_right:   upper limit of interval of the node points.
     */
    void update_query(ll left, ll right, ll val, ll pos = 0, ll node_left = 0, ll node_right = -1) {
        if(node_right < 0) {
            node_right = N;
        }

        /*
         * Execute lazy evaluation.
         */
        lazy_eval(pos, node_left, node_right);

        /*
         * If the node is out of inrerval, return.
         */
        if(right <= node_left || node_right <= left) {
            return;
        }


        /*
         * If the node cover the interval complety, update this->lazy and execute lazy_eval.
         * Else recursion.
         */
        if(left <= node_left && node_right <= right) {
            lazy[pos] = create_lazy_value(node_left, node_right, val);
            lazy_flag[pos] = true;
            lazy_eval(pos, node_left, node_right);
        }else{

            /*
             * recursion
             */
            update_query(left, right, val, 2 * pos + 1, node_left, (node_left + node_right) / 2);
            update_query(left, right, val, 2 * pos + 2, (node_left + node_right) / 2, node_right);

            node[pos] = merge_node(node[2 * pos + 1], node[2 * pos + 2]);
        }
    }

    ll get_query(ll left, ll right, ll pos = 0, ll node_left = 0, ll node_right = -1) {
        if(node_right < 0) {
            node_right = N;
        }

        /*
         * Evaluate the node[pos]
         */
        lazy_eval(pos, node_left, node_right);

        if(node_right <= left || right <= node_left) {
            return init_node;
        }
        if(left <= node_left && node_right <= right) {
            return node[pos];
        }

        ll split = (node_left + node_right) / 2;
        return merge_node(get_query(left, right, 2 * pos + 1, node_left, split),
                          get_query(left, right, 2 * pos + 2, split, node_right));
    }
};

int main() {
    // solution for http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F
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
        }else{
            ll s, t;
            cin >> s >> t;
            cout << LST.get_query(s, t + 1) << '\n';
        }
    }
    return 0;
}
