#include "../util/template.hpp"

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
};

template <typename Node>
bool is_l(const Node *n) {
    return !n->is_root() && n->p->l == n;
}

/*
 * - counter clock wise
 *    p             x
 *   / \           / \
 *  a   x    ->   p   c
 *     / \       / \
 *    b   c     a   b
 *
 * - clock wise
 *      p           x         
 *     / \         / \
 *    x   c  ->   a   p 
 *   / \             / \
 *  a   b           b   c     
 *
 */
template <typename Node>
void rotate(Node *n, bool cc) {
    auto x = n, p = n->p;
    auto pp = p->p;
    Node *a, *b, *c;
    bool rp = p->is_root();  // for lctree
    bool ppl = is_l(p);
    if (cc) {
        a = p->l; b = x->l; c = x->r;
        x->l = p; x->r = c; x->p = pp;
        p->l = a; p->r = b; p->p = x;
        if (pp && !rp) {
            if (ppl) pp->l = x;
            else pp->r = x;
        }
    } else {
        a = x->l; b = x->r; c = p->r;
        x->l = a; x->r = p; x->p = pp;
        p->l = b; p->r = c; p->p = x;
        if (pp && !rp) {
            if (ppl) pp->l = x;
            else pp->r = x;
        }
    }
    p->update();
    x->update();
    if (pp) pp->update();
}

template <typename Node>
void zig(Node *n) {
    rotate(n, !is_l(n));
}

template <typename Node>
void zig_zig(Node *n) {
    bool wise = !is_l(n);
    rotate(n->p, wise);
    rotate(n, wise);
}

template <typename Node>
void zig_zag(Node *n) {
    bool wise = !is_l(n);
    rotate(n, wise);
    rotate(n, !wise);
}

template <typename Node>
void splay_aux(Node *n) {
    auto p = n->p;
    if (!p) return;
    if (p->is_root()) {
        zig(n);
        return;
    }
    bool a = is_l(n);
    bool b = is_l(p);
    if (a == b) zig_zig(n);
    else zig_zag(n);
}

template <typename Node>
void splay(Node *n) {
    while (!n->is_root()) splay_aux(n);
}

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
