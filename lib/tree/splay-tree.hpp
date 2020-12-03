#pragma once
#include "bbst-base.hpp"

namespace tree {

template <typename Key>
struct splay_node {
    using node_ptr = splay_node<Key>*;
    using size_type = ssize_t;

    Key key;
    node_ptr l, r, p;
    size_type sz;

    splay_node(Key key, node_ptr l, node_ptr r, node_ptr p) : key(key), l(l), r(r), p(p), sz(1) { }
    splay_node(Key key) : splay_node(key, nullptr, nullptr, nullptr) { }
    splay_node() : splay_node(Key()) { }

    bool is_root() const {
        return !p;
    }

    void update() {
        sz = 1;
        if (l) {
            sz += l->sz;
            l->p = this;
        }
        if (r) {
            sz += r->sz;
            r->p = this;
        }
    }
    
    void rotate(bool cc) {
        rotate_aux(this, cc);
    }
};


template <typename Key, typename Comp>
struct SplayTree {
    using node = splay_node<Key>;
    using size_type = typename node::size_type;
    using node_ptr = typename node::node_ptr;

    node_ptr root;
    Comp comp;

    SplayTree(node_ptr root = nullptr) : root(root) { }

    void splay(node_ptr n) {
        splay(n);
        root = n;
    }

    node_ptr find(Key key) {
        node_ptr cur = root;
        while (cur) {
            if (cur->key == key) break;
            if (comp(key, cur->key)) {
                if (!cur->l) break;
                cur = cur->l;
            } else {
                if (!cur->r) break;
                cur = cur->r;
            }
        }
        this->splay(cur);
        if (cur->key != key) return nullptr;
        else return cur;
    }

    node_ptr insert(Key key) {
        if (root == nullptr) return root = new node(key);
        find(key);
        if (root->key == key) return root;
        node_ptr n = new node(key);
        if (comp(root->key, key)) {
            root->p = n;
            n->l = root;
            n->r = root->r;
            root->r = nullptr;
        } else {
            root->p = n;
            n->l = root->l;
            n->r = root;
            root->l = nullptr;
        }
        if (n->l) n->l->update();
        if (n->r) n->r->update();
        n->update();
        root = n;
        return root;
    }

    void erase(node_ptr n) {
        if (!n) return;
        this->splay(n);
        node_ptr l = root->l, r = root->r;
        n->l = n->r = nullptr;
        if (l) l->p = nullptr;
        if (r) r->p = nullptr;
        if (!l && !r) {
            root = nullptr;
        } else if (!l) {
            root = r;
        } else if (!r) {
            root = l;
        } else {
            SplayTree tmp(r);
            tmp.find(l->key);
            node_ptr rmin = tmp.root;
            rmin->l = l;
            rmin->update();
            root = rmin;
        }
        if (root) root->update();
        delete n;
    }

    void erase(Key key) {
        node_ptr n = find(key);
        erase(n);
    }

    node_ptr operator [](size_type idx) {
        size_type sz = idx + 1;
        node_ptr cur = root;
        while (true) {
            size_type lsz = (cur->l ? cur->l->sz : 0);
            size_type ord = lsz + 1;
            if (sz == ord) {
                break;
            } else if (sz < ord) {
                cur = cur->l;
            } else {
                sz -= ord;
                cur = cur->r;
            }
        }
        this->splay(cur);
        return root;
    }
};

}
