#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;
using ull = uint64_t;

template <typename Key>
struct SplayTree {
    template <typename T>
    struct Node__ {
        using node_ptr__ = shared_ptr<Node__<T>>;
        node_ptr__ parent, left, right;
        size_t subtree_size;
        T key;
        Node__(T key, node_ptr__ parent, node_ptr__ left, node_ptr__ right) : key(key), parent(parent), left(left), right(right), subtree_size(1) {}
        Node__(T key) : Node__(key, nullptr, nullptr, nullptr) {}
        Node__() : Node__(T()) {}
    };

    using Node = Node__<Key>;
    using node_ptr = shared_ptr<Node>;
    using Compare = function<bool(Key, Key)>;
    Compare comp;
    node_ptr root;

    SplayTree(Compare comp)
        : comp(comp),
          root(nullptr)
    {
    }

    bool is_root(node_ptr node) { return node == nullptr || node->parent == nullptr; }
    bool is_left(node_ptr node) { return node->parent->left == node; }
    size_t get_subtree_size(node_ptr node) { return node == nullptr ? 0 : node->subtree_size; }
    void update_subtree_size(node_ptr node) { if(node) node->subtree_size = get_subtree_size(node->left) + get_subtree_size(node->right) + 1; }
    void set_root(node_ptr node) { root = node; if(root) root->parent = nullptr; }
    void update_root(node_ptr node) { if(!node->parent) root = node; }

    void rotate(node_ptr cur_node, bool left) {
        auto par_node = cur_node->parent;
        set_by_key(par_node->parent, cur_node);
        if(left) {
            set_right(par_node, cur_node->left);
            set_left(cur_node, par_node);
        } else {
            set_left(par_node, cur_node->right);
            set_right(cur_node, par_node);
        }
        update_subtree_size(par_node);
        update_subtree_size(cur_node);
        update_root(cur_node);
    }

    void set_by_key(node_ptr par, node_ptr ch) {
        if(ch) ch->parent = par;
        if(par && ch) {
            if(comp(par->key, ch->key)) set_right(par, ch);
            else set_left(par, ch);
        }
    }

    void set_left(node_ptr par, node_ptr ch) {
        if(par) par->left = ch;
        if(ch) ch->parent = par;
    }

    void set_right(node_ptr par, node_ptr ch) {
        if(par) par->right = ch;
        if(ch) ch->parent = par;
    }

    void set_children(node_ptr par, node_ptr left, node_ptr right) {
        set_left(par, left);
        set_right(par, right);
    }

    // is_root(get_parent(idx)) must be true
    // rotate_right is execed if is_left(cur_node) is true.
    void zig(node_ptr cur_node, bool left) {
        rotate(cur_node, !left);
    }

    void zig_zig(node_ptr cur_node, bool left) {
        rotate(cur_node->parent, !left);
        rotate(cur_node, !left);
    }

    void zig_zag(node_ptr cur_node, bool left) {
        rotate(cur_node, !left);
        rotate(cur_node, left);
    }

    void splay(node_ptr node) {
        while(!is_root(node)) {
            if(is_root(node->parent)) {
                zig(node, is_left(node));
                break;
            }
            bool cur_state = is_left(node);
            bool par_state = is_left(node->parent);
            if(cur_state ^ par_state) zig_zag(node, cur_state);
            else zig_zig(node, cur_state);
        }
    }

    node_ptr search(Key key) {
        node_ptr cur_node = root;
        while(true) {
            if(cur_node->key == key) break;
            if(comp(key, cur_node->key)) {
                if(cur_node->left) cur_node = cur_node->left;
                else break;
            } else {
                if(cur_node->right) cur_node = cur_node->right;
                else break;
            }
        }
        return cur_node;
    }

    node_ptr find(Key key) {
        auto ret = search(key);
        splay(ret);
        return ret->key == key ? ret : nullptr;
    }

    void insert(Key key) {
        if(root == nullptr) {
            root = make_shared<Node>(key, nullptr, nullptr, nullptr);
            return;
        }
        auto node = search(key);
        if(node->key != key) {
            auto nnode = make_shared<Node>(key, nullptr, nullptr, nullptr);
            set_by_key(node, nnode);
            node = nnode;
        }
        splay(node);
    }

    void erase(Key key) {
        auto node = search(key);
        if(node->key != key) return;
        splay(node);
        if(!root->left) {
            set_root(root->right);
        } else if(!root->left->right) {
            set_right(root->left, root->right);
            set_root(root->left);
        } else {
            node_ptr nxt_root = root->left;
            while(nxt_root->right) {
                nxt_root->subtree_size--;
                nxt_root = nxt_root->right;
            }
            set_right(nxt_root->parent, nxt_root->left);
            
            set_children(nxt_root, root->left, root->right);
            set_root(nxt_root);
        }
        update_subtree_size(root);
    }

    // this method is defined for https://atcoder.jp/contests/arc033/tasks/arc033_3
    Key query(ll X) {
        auto cur_node = root;
        while(X) {
            size_t left_size = get_subtree_size(cur_node->left);
            if(left_size == X - 1) break;
            if(left_size + 1 < X) {
                X -= left_size + 1;
                cur_node = cur_node->right;
            } else {
                cur_node = cur_node->left;
            }
        }
        auto ret = cur_node->key;
        splay(cur_node);
        erase(ret);
        return ret;
    }

    void dump(node_ptr node, string label = "") {
        cout << label << " : " << node->key << ", " << get_subtree_size(node) << endl;
        if(node->left) dump(node->left, label + 'l');
        if(node->right) dump(node->right, label + 'r');
    }

    void dump() {
        cout << "==== begin dump ====\n";
        dump(root);
        cout << "==== end dump ====\n";
    }
};

// solution for https://atcoder.jp/contests/arc033/tasks/arc033_3

#define VERIFY
#ifdef VERIFY
int main() {
    SplayTree<ll> sp_tree([](ll a, ll b) { return a < b; });
    ll Q;
    cin >> Q;
    while(Q--) {
        ll T, X;
        cin >> T >> X;
        if(T == 1) sp_tree.insert(X);
        else cout << sp_tree.query(X) << endl;
    }
    return 0;
}
#endif
