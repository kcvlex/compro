#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
template <typename T> using V = vector<T>;

template <typename T>
class DynamicSegmentTree {
    
    template <typename U>
    struct Node {
        ll left_idx, right_idx, parent_idx;
        U val;
        Node(U val) : val(val), left_idx(-1), right_idx(-1), parent_idx(-1) {}
    };

    V<Node<T>> nodes_heap;
    ll heap_idx;

    T sub_query(ll a, ll b, ll node_idx, ll l, ll r) {
        if(r <= a || b <= l) return identity_ele;
        if(node_idx == -1) return identity_ele;
        
        const auto &node = nodes_heap[node_idx];
        
        if(a <= l && r <= b) {
            return node.val;
        } else {
            T left = sub_query(a, b, node.left_idx, l, (l + r) / 2);
            T right = sub_query(a, b, node.right_idx, (l + r) / 2, r);
            return comp(left, right);
        }
    }

    ll create_node(T val) {
        nodes_heap[heap_idx] = Node<T>(val);
        ll ret = heap_idx;
        heap_idx++;
        return ret;
    }

    ll update_child_node(ll node_idx, bool left) {
        auto &node = nodes_heap[node_idx];
        ll &idx = (left ? node.left_idx : node.right_idx);
        if(idx == -1) idx = create_node(identity_ele);
        auto &nxt = nodes_heap[idx];
        nxt.parent_idx = node_idx;
        return idx;
    }

public:
    function<T(T, T)> comp;
    T identity_ele;  //comp(x, identity_ele) == x
    ll sup;

    DynamicSegmentTree(ll sup, function<T(T, T)> comp, 
                       T identity_ele, size_t heap_size = 2e5)
        : comp(comp),
          identity_ele(identity_ele),
          nodes_heap(heap_size, identity_ele),
          heap_idx(1),
          sup(1)
          
    {
        while(this->sup < sup) (this->sup) *= 2;
    }

    void update(ll pos, T value) {
        ll node_idx = 0;
        ll left = 0, right = sup;
        ll parent_idx = -1; 
        while(right - left > 1) {
            ll mid = (left + right) / 2;
            parent_idx = node_idx;
            if(pos < mid) {
                node_idx = update_child_node(node_idx, true);
                right = mid;
            } else {
                node_idx = update_child_node(node_idx, false);
                left = mid;
            }
        }

        nodes_heap[node_idx] = value;
        nodes_heap[node_idx].parent_idx = node_idx;

        while(parent_idx != -1) {
            auto &node = nodes_heap[parent_idx];
            ll lidx = node.left_idx;
            ll ridx = node.right_idx;
            T left  = (lidx == -1 ? identity_ele : nodes_heap[lidx].val);
            T right = (ridx == -1 ? identity_ele : nodes_heap[ridx].val);
            node.val = comp(left, right);
            parent_idx = node.parent_idx;
        }
    }

    T query(ll a, ll b) {
        return sub_query(a, b, 0, 0, sup);
    }
};

// solution for https://arc008.contest.atcoder.jp/tasks/arc008_4

template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return max(t, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }

using PDD = pair<double, double>;

int main() {
    cout << fixed << setprecision(30);
    ll N, M;
    cin >> N >> M;

    DynamicSegmentTree<PDD> st(N, 
                               [](PDD p, PDD q) {
                                   double a, b, c, d;
                                   tie(a, b) = p;
                                   tie(c, d) = q;
                                   return PDD(a * c, b * c + d);
                               },
                               PDD(1.0, 0.0), 5e6);

    double maxv = 1, minv = 1;
    for(ll i = 0; i < M; i++) {
        ll p;
        double a, b;
        cin >> p >> a >> b;
        p--;
        st.update(p, PDD(a, b));
        PDD root = st.query(0, N);
        double val = root.first + root.second;
        chmax(maxv, val);
        chmin(minv, val);
    }

    cout << minv << endl
         << maxv << endl;
    return 0;
}

