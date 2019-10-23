#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;
using ull = uint64_t;

#define VERIFY

template <typename Key>
struct SplayTreeNode {
    using node_ptr = SplayTreeNode<Key>*;
    node_ptr parent, left, right;
    Key key;
    size_t size;
    
    SplayTreeNode(Key key, node_ptr parent) : key(key), parent(parent), left(nullptr), right(nullptr), size(1) {}
    SplayTreeNode(Key key) : SplayTreeNode(key, nullptr) {}
    SplayTreeNode() : SplayTreeNode(Key()) {}

    bool is_left() const {
        return parent != nullptr && parent->left == this;
    }

    bool is_root() const {
        return parent == nullptr;
    }

    void rotate(bool left_ch) {
        if(is_root()) return;
        auto p = parent;
        auto pp = p->parent;
        parent = pp;
        if(pp != nullptr) (p->is_left() ? pp->left : pp->right) = this;
        if(left_ch) {
            p->left = right;
            if(right != nullptr) right->parent = p;
            right = p;
            p->parent = this;
        } else {
            p->right = left;
            if(left != nullptr) left->parent = p;
            left = p;
            p->parent = this;
        }
        p->update_size();
        update_size();
    }

    void update_size() {
        size = 1;
        if(left != nullptr) size += left->size;
        if(right != nullptr) size += right->size;
    }

    void rotate() {
        rotate(is_left());
    }

    void zig() {
        rotate();
    }

    void zig_zig() {
        parent->rotate();
        rotate();
    }

    void zig_zag() {
        rotate();
        rotate();
    }

    void splay() {
        while(!is_root()) {
            if(parent->is_root()) rotate();
            else if(is_left() ^ parent->is_left()) zig_zag();
            else zig_zig();
        }
    }
};

template <typename Key>
struct SplayTree {
    using Node = SplayTreeNode<Key>;
    using node_ptr = typename SplayTreeNode<Key>::node_ptr;
    using Compare = function<bool(Key, Key)>;
    Compare comp;  // min
    node_ptr root;

    SplayTree(Compare comp) : comp(comp), root(nullptr) {}

    node_ptr find(Key key) {
        search(key);
        return (root != nullptr && root->key == key) ? root : nullptr;
    }

    void insert(Key key) {
        if(root == nullptr) {
            root = new Node(key);
            return;
        }
        node_ptr node = search(key);
        if(node->key != key) {
            node_ptr new_node = new Node(key, node);
            (comp(key, node->key) ? node->left : node->right) = new_node;
            new_node->parent = node;
            splay(new_node);
        } else {
            splay(node);
        }
    }

    void erase(node_ptr node) {
        splay(node);
        if(node->left == nullptr) {
            root = node->right;
            if(node->right != nullptr) node->right->parent = nullptr;
        } else if(node->left->right == nullptr) {
            root = node->left;
            root->right = node->right;
            if(node->right != nullptr) node->right->parent = root;
            root->parent = nullptr;
            root->update_size();
        } else {
            assert(node == root);
            node_ptr cur = node->left;
            while(cur->right != nullptr) cur = cur->right;
            auto bottom = cur->parent;
            root = cur;
            root->parent = nullptr;
            bottom->right = root->left;
            if(root->left != nullptr) root->left->parent = bottom;
            root->left = node->left;
            root->right = node->right;
            node->left->parent = root;
            if(node->right != nullptr) node->right->parent = root;
            while(bottom != nullptr) {
                bottom->update_size();
                bottom = bottom->parent;
            }
        }
    }

    void erase(Key key) {
        node_ptr node = search(key);
        if(node->key != key) return;
        erase(node);
    }

    ll query(size_t ord) {
        size_t cnt = ord;
        node_ptr cur = root;
        ll ret = 0;
        while(cnt) {
            auto left_size = (cur->left == nullptr ? 0 : cur->left->size);
            if(left_size + 1 == cnt) {
                ret = cur->key;
                break;
            }
            if(left_size + 1 < cnt) {
                cnt -= left_size + 1;
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }
        erase(cur);
        return ret;
    }

    void dump() { dump(root); }

    void splay(node_ptr node) {
        node->splay();
        root = node;
    }

private:
    node_ptr search(Key key) {
        node_ptr cur = root;
        while(cur != nullptr) {
            if(cur->key == key) break;
            if(comp(key, cur->key)) {
                if(cur->left) cur = cur->left;
                else break;
            } else {
                if(cur->right) cur = cur->right;
                else break;
            }
        }
        return cur;
    }
};

// solution for https://atcoder.jp/contests/arc033/tasks/arc033_3
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
