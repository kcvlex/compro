// #define DEBUGGING
#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define ALL(V) (V).begin(), (V).end()
#define ALLR(V) (V).rbegin(), (V).rend()

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
template <typename T, typename U> using P = pair<T, U>;
using ll = int64_t;
using ull = uint64_t;
using PLL = P<ll, ll>;

template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return max(t, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }
template <typename T> const T& clamp(const T &t, const T &low, const T &high) { return max(low, min(high, t)); }
template <typename T> void chclamp(T &t, const T &low, const T &high) { t = clamp(t, low, high); }

namespace init__ {

struct InitIO {
    InitIO() {
        cin.tie(nullptr);
        ios_base::sync_with_stdio(false);
        cout << fixed << setprecision(30);
    }
} init_io;

}

#ifdef DEBUGGING
#include "../debug.cpp"
#else
#define DEBUG(...) 0
#define DEBUG_SEPARATOR_LINE 0
#endif

template <typename T>
T make_v(T init) { return init; }

template <typename T, typename... Tail>
auto make_v(T init, size_t s, Tail... tail) {
#define rec make_v(init, tail...)
    return V<decltype(rec)>(s, rec);
#undef rec
}

template <typename Label, size_t MaxChildren>
struct Trie {
    template <typename T, size_t Size>
    struct Node__ {
        ull depth;
        array<size_t, Size> children_idx;
        size_t parent_idx;
        pair<bool, T> status;

        Node__(ull depth, size_t parent_idx) : depth(depth), parent_idx(parent_idx), status(false, T()) { for(auto &ele : children_idx) ele = numeric_limits<size_t>::max(); } 
        Node__() : Node(0, numeric_limits<size_t>::max()) {}
        bool has_child(size_t idx) const { return children_idx[idx] != numeric_limits<size_t>::max(); }
    };

    using Node = Node__<V<Label>, MaxChildren>;

    V<Node> nodes_loc;
    size_t node_loc_idx;
    const size_t root_idx;
    const function<size_t(const Label&)> label_map;

    Trie(function<size_t(const Label&)> label_map, size_t max_loc_size = 1e6) 
        : nodes_loc(max_loc_size), 
          node_loc_idx(1),
          root_idx(0),
          label_map(label_map)
    {
    }

    Node& get_root() { return nodes_loc[root_idx]; }
    Node& get_node(size_t idx) { return nodes_loc[idx]; }

    enum LabelStatus { Continue, Finish };
    using LabelInfo = pair<LabelStatus, Label>;
    using IterateLabel = function<LabelInfo()>;
    
    template <typename Iterator>
    struct IteratorWrapper {
        Iterator current, last;
        IteratorWrapper(Iterator current, Iterator last) : current(current), last(last) {}
        LabelInfo operator()() {
            if(current == last) return make_pair(LabelStatus::Finish, Label());
            auto label = *current;
            advance(current, 1);
            return make_pair(LabelStatus::Continue, label);
        }
    };

    void create_node(IterateLabel &ite_label, size_t node_depth, size_t node_idx, V<Label> &&history) {
        auto label_info = ite_label();
        if(label_info.first == LabelStatus::Finish) {
            get_node(node_idx).status = make_pair(true, move(history));
            return;
        }

        auto &node = get_node(node_idx);
        const Label &label = label_info.second;
        size_t label_id = label_map(label);
        history.push_back(label);

        if(!node.has_child(label_id)) {
            size_t loc_idx = node_loc_idx++;
            nodes_loc[loc_idx] = Node(node_depth + 1, node_idx);
            node.children_idx[label_id] = loc_idx;
        }
        
        create_node(ite_label, node_depth + 1, node.children_idx[label_id], move(history));
    }

    size_t search_node(IterateLabel &ite_label, size_t node_idx) {
        auto &node = get_node(node_idx);
        auto label_info = ite_label();
        if(label_info.first == LabelStatus::Finish) return node.depth;

        const Label &label = label_info.second;
        size_t label_id = label_map(label);

        if(!node.has_child(label_id)) return node.depth;
        else return search_node(ite_label, node.children_idx[label_id]);
    }

    template <typename Iterator> IterateLabel create_ite_label(Iterator current, Iterator last) { return IteratorWrapper<Iterator>(current, last); }

    void create_node(IterateLabel &ite_label) { create_node(ite_label, 0, root_idx, V<Label>()); }
    size_t search_node(IterateLabel &ite_label) { return search_node(ite_label, root_idx); }
    template <typename T> void create_node(const T &t) { auto ite_label = create_ite_label(ALL(t)); create_node(ite_label); }
    template <typename T> size_t search_node(const T &t) { auto ite_label = create_ite_label(ALL(t)); return search_node(ite_label); }
};

// solution for https://atcoder.jp/contests/arc087/tasks/arc087_c
int main() {
    ll N, L;
    cin >> N >> L;
    V<ll> imos(1e5 + 10);
    imos[0] = 2;
    Trie<char, 2> trie([](char c) { return c - '0'; });
    
    for(ll i = 0; i < N; i++) {
        string s;
        cin >> s;
        auto d = trie.search_node(s);
        DEBUG(d);
        imos[d]--;
        imos[d + 1] += 1 + (d != 0);
        imos[s.size()]--;
        trie.create_node(s);
    }
 
    ll gxor = 0;
    for(ll i = 1; i < imos.size() - 1; i++) imos[i + 1] += imos[i];

    for(ll i = 0; i < imos.size(); i++) {
        if(imos[i] % 2 == 0) continue;
        bitset<64> bs(L - i);
        for(ll j = 0; j <= 63; j++) {
            if(!bs.test(j)) continue;
            gxor ^= (1ll << j);
            break;
        }
    }
    cout << (gxor ? "Alice" : "Bob") << endl;
    return 0;
}
