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
    V<Label> label_list;

    Trie(function<size_t(const Label&)> label_map, size_t max_loc_size = 1e6) 
        : nodes_loc(max_loc_size), 
          node_loc_idx(1),
          root_idx(0),
          label_map(label_map)
    {
    }

    void set_label_list(V<Label> label_list) { this->label_list = label_list; }
    const V<Label>& get_label_list() { assert(!label_list.empty()); return label_list; }

    size_t get_node_count() const { return node_loc_idx; }
    Node& get_root() { return nodes_loc[root_idx]; }
    Node& get_node(size_t idx) { return nodes_loc[idx]; }

    enum LabelStatus { Continue, Finish };
    using LabelInfo = pair<LabelStatus, Label>;
    using IterateLabel = function<LabelInfo()>;
    
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

    IterateLabel create_ite_label(const string &str) {
        static size_t idx = 0;
        idx = 0;
        auto ret = [&] {
            if(str.size() <= idx) return make_pair(LabelStatus::Finish, static_cast<char>(0));
            char c = str[idx];
            idx++;
            return pair<LabelStatus, char>(LabelStatus::Continue, c);
        };
        return ret;
    }

    IterateLabel create_ite_label(const V<Label> &vec) {
        static size_t idx = 0;
        idx = 0;
        auto ret = [&] {
            if(vec.size() <= idx) return make_pair(LabelStatus::Finish, Label());
            Label t = vec[idx];
            idx++;
            return make_pair(LabelStatus::Continue, t);
        };
        return ret;
    }

    void create_node(IterateLabel &ite_label) { create_node(ite_label, 0, root_idx, V<Label>()); }
    size_t search_node(IterateLabel &ite_label) { return search_node(ite_label, root_idx); }
    template <typename T> void create_node(const T &t) { auto ite_label = create_ite_label(t); create_node(ite_label); }
    template <typename T> size_t search_node(const T &t) { auto ite_label = create_ite_label(t); return search_node(ite_label); }
};

template <typename Label, size_t MaxChildren>
struct AhoCorasick {
    Trie<Label, MaxChildren + 1> trie;
    const size_t failed_idx;
    V<ull> count_correct;
    V<string> accepts_id_to_str;
    VV<ll> accepts_id_list;

    AhoCorasick(function<size_t(const Label&)> label_map, size_t max_loc_size = 1e6) 
        : trie(label_map, max_loc_size), 
          failed_idx(MaxChildren),
          accepts_id_list(max_loc_size),
          accepts_id_to_str(max_loc_size)
    {
        count_correct.assign(trie.get_node_count(), 0);
        V<string> accepts_str(trie.node_loc);
        for(size_t i = 0; i < trie.node_loc; i++) {
            auto &node = trie.get_node(i);
            if(!node.status.first) continue;
            accepts_str[i] = node.status.second;
            accepts_id_to_str.push_back(accepts[i]);
        }

        {
            sort(ALL(accepts_id_to_str));
            auto ite = unique(ALL(accepts_id_to_str));
            accepts_id_to_str.erase(ite, accepts_id_to_str.end());
        }

        for(size_t i = 0; i < trie.node_loc; i++) if(accepts_str[i].size()) {
            accepts_id_list[i].push_back(make_accept_id(accepts_str[i]));
        }
    }

    size_t make_accept_id(const string &s) {
        return distance(accepts_idx_to_str.begin(),
                        lower_bound(ALL(accepts_idx_to_str), s));
    }

    queue<size_t> init_bfs() {
        queue<size_t> que;
        for(size_t idx = 0; idx < failed_idx; idx++) {
            if(trie.get_root().has_child(idx)) {
                auto &nxt_node = trie.get_node(trie.get_root().children_idx[idx]);
                nxt_node.children_idx[failed_idx] = trie.root_idx;
                que.push(idx);
            } else {
                get_root().children_idx[idx] = trie.root_idx;
            }
        }
        return que;
    }

    size_t suffix_link_node_idx(size_t current_node_idx, size_t label_id) {
        size_t idx = trie.get_node(current_node_idx).children_idx[failed_idx];
        while(trie.get_node(idx).children_idx[label_id] == -1) idx = trie.get_node(idx).children_idx[label_id];
        return idx;
    }

    template <typename T>
    void create_trie(const V<T> &dictionary) {
        for(const auto &ele : dictionary) trie.create_node(ele);

        auto que = init_bfs();
        while(que.size()) {
            auto current_idx = que.front();
            que.pop();
            const auto &current_node = trie.get_node(current_idx);
            count_correct[current_idx] += count_correct[current_node.children_idx[failed_idx]];
            for(size_t label_id = 0; label_id < MaxChildren; label_id++) {
                if(!current_node.has_child(label_id)) continue;
                size_t next_idx = current_node.children_idx[label_id];
                auto &next_node = trie.get_node(next_idx);
                next_node.children_idx[failed_idx] = suffix_link_node_idx(current_idx, label_id);
                auto &next_node_fail = trie.get_node(next_node.children_idx[failed_idx]);

                V<ll> accept_v;
                set_union(ALL(accepts_str_idx[next_idx]), ALL(accepts_str_idx[next_node_fail.next_children[label_id]]), back_insert(accept_v));
                accepts_str[next_idx] = move(accept_v);

                que.push(next_idx);
            }
        }
    }

    V<ull> query(const string &str) {
        V<uint64_t> res;
        auto cur_idx = 0;
        for(auto &&c : str) {
            auto label_id = label_map(c);
            do {
                cur_idx = trie.get_node(cur_idx).children_idx[label_id];
            } while(cur_idx == failed_idx);
            for(auto &&acc_idx : accepts[cur_idx]) res[acc_idx]++;
        }
        return res;
    }

};
