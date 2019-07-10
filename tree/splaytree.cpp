#include <bits/stdc++.h>
using namespace std;

using ll = int64_t;
template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;

template <typename Key>
struct SplayTree {
    template <typename T>
    struct Node__ {
        using node_ptr__ = shared_ptr<Node__<T>>;
        node_ptr__ parent, left, right;
        T key;
        Node__(T key, node_ptr__ parent, node_ptr__ left, node_ptr__ right) : key(key), parent(parent), left(left), right(right) {}
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

    bool is_root(node_ptr node) { return node->parent == nullptr; }
    bool is_left(node_ptr node) { return node->parent->left == node; }
    void update_root(node_ptr node) { if(node->parent == nullptr) root = node; }

    node_ptr make_node(Key key, node_ptr parent, node_ptr left, node_ptr right) {
        node_ptr ret = make_shared<Node>(key, parent, left, right);
        return ret;
    }

    // is_root(idx) must not be true
    void rot_right(node_ptr cur_node) {
        auto par_node = cur_node->parent;
        cur_node->parent = par_node->parent;
        par_node->parent = cur_node;
        par_node->left = cur_node->right;
        cur_node->right = par_node;
        update_root(cur_node);
    }

    void rot_left(node_ptr cur_node) {
        auto par_node = cur_node->parent;
        cur_node->parent = par_node->parent;
        par_node->parent = cur_node;
        par_node->right = cur_node->left;
        cur_node->left = par_node;
        update_root(cur_node);
    }

    // is_root(get_parent(idx)) must be true
    // rot_right is execed if par.left == idx
    void zig(node_ptr cur_node, bool left) {
        if(left) rot_right(cur_node);
        else rot_left(cur_node);
    }

    void zig_zig(node_ptr cur_node, bool left) {
        auto func = (left ? rot_right : rot_left);
        func(cur_node->parent);
        func(cur_node);
    }

    void zig_zag(node_ptr cur_node, bool left) {
        auto fst_func = (left ? rot_right : rot_left);
        auto snd_func = (left ? rot_left : rot_right);
        fst_func(cur_node);
        snd_func(cur_node);
    }

    void splay(node_ptr cur_node) {
        while(cur_node != root) {
            if(root->left == cur_node) {
                zig(cur_node, left);
                break;
            } else if(root->right == cur_node) {
                zig(cur_node, right);
                break;
            }
            auto par_node = cur_node->parent;
            bool cur_left = is_left(cur_node);
            bool par_left = is_left(par_node);
            if(cur_left ^ par_left) zig_zag(cur_node, cur_left);
            else zig_zig(cur_node, cur_left);
        }
    }

    node_ptr find(Key key) {
        auto cur_node = root;
        while(true) {
            if(cur_node.key == key) break;
            if(comp(key, cur_node->key) && cur_node->left != nullptr) {
                auto nxt_node = cur_node->left;
                if(nxt_node.key != key && comp(nxt_node.key, key)) break;
                else cur_node = nxt_node;
            } else if(!comp(key, cur_node->key) && cur_node->right != nullptr) {
                cur_node = cur_node->right;
            } else {
                break;
            }
        }
        splay(cur_node);
        return cur_node;
    }

    node_ptr find_max(node_ptr cur_node) {
        auto ret = cur_node;
        while(ret->right) ret = ret->right;
        return ret;
    }

    void insert(Key key) {
        if(root == nullptr) {
            root = make_node(key, nullptr, nullptr, nullptr);
            return;
        }
        if(find(key)) return;
        auto node = make_node(key, nullptr, nullptr, nullptr);
        root->parent = node;
        if(comp(key, root->key)) {
            node->left = root->left;
            root->left = nullptr;
            node->right = root;
        } else {
            node->right = root->right;
            root->right = nullptr;
            node->left = root;
        }
    }

    void erase(Key key) {
        auto lower = find(key);
        if(!find(key)) return;
        if(!root->left) {
            root = root->right;
            root->parent = nullptr;
        } else {
            auto left_max = find_max(root->left);
            left_max->parent->left = nullptr;
            left_max->left = root->left;
            left_max->right = root->right;
            left_max->parent = nullptr;
            root = left_max;
        }
    }
};
