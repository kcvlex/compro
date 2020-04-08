#include "../util/template.cpp"

namespace tree {

template <typename Key>
struct splay_node {
    using node_ptr = splay_node<Key>*;
    Key key;
    node_ptr l, r, p;
    ssize_t sz;
    splay_node(Key key, node_ptr l, node_ptr r, node_ptr p) : key(key), l(l), r(r), p(p), sz(1) { }
    splay_node(Key key) : splay_node(key, nullptr, nullptr, nullptr) { }
    splay_node() : splay_node(Key()) { }

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

    void rotate(bool cc) {
        node_ptr x = this, p = this->p;
        node_ptr pp = p->p, a, b, c;
        bool ppl = p->is_l();
        if (cc) {
            a = p->l, b = x->l, c = x->r;
            x->l = p, x->r = c, x->p = pp;
            p->l = a, p->r = b, p->p = x;
            if (pp) {
                if (ppl) pp->l = x;
                else pp->r = x;
            }
        } else {
            a = x->l, b = x->r, c = p->r;
            x->l = a, x->r = p, x->p = pp;
            p->l = b, p->r = c, p->p = x;
            if (pp) {
                if (ppl) pp->l = x;
                else pp->r = x;
            }
        }
        p->update();
        x->update();
        if (pp) pp->update();
    }

    bool is_root() const {
        return p == nullptr;
    }

    bool is_l() const {
        return !is_root() && p->l == this;
    }

    void zig() {
        rotate(!is_l());
    }

    void zig_zig() {
        bool wise = !is_l();
        p->rotate(wise);
        rotate(wise);
    }

    void zig_zag() {
        bool wise = !is_l();
        rotate(wise);
        rotate(!wise);
    }

    void splay() {
        while (!is_root()) {
            node_ptr p = this->p;
            if (!p) break;
            node_ptr pp = p->p;
            if (!pp) {
                zig();
                break;
            }
            bool a = this->is_l();
            bool b = p->is_l();
            if (a == b) zig_zig();
            else zig_zag();
        }
    }
};

template <typename Key>
struct SplayTree {
    using node = splay_node<Key>;
    using node_ptr = typename node::node_ptr;

    node_ptr root;

    SplayTree(node_ptr root = nullptr) : root(root) { }

    void splay(node_ptr n) {
        n->splay();
        root = n;
    }

    node_ptr find(Key key) {
        node_ptr cur = root;
        while (cur) {
            if (cur->key == key) break;
            if (key < cur->key) {
                if (!cur->l) break;
                cur = cur->l;
            } else {
                if (!cur->r) break;
                cur = cur->r;
            }
        }
        splay(cur);
        if (cur->key != key) return nullptr;
        else return cur;
    }

    node_ptr insert(Key key) {
        if (root == nullptr) return root = new node(key);
        find(key);
        if (root->key == key) return root;
        node_ptr n = new node(key);
        if (root->key < key) {
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
        splay(n);
        node_ptr l = root->l, r = root->r;
        n->l = n->r = nullptr;
        if (l) l->p = nullptr;
        if (r) r->p = nullptr;
        if (!l && !r) {
            root = nullptr;
        } else if (!l) {
            root = r;
            root->update();
        } else if (!r) {
            root = l;
            root->update();
        } else {
            SplayTree tmp(r);
            tmp.find(l->key);
            node_ptr rmin = tmp.root;
            DEBUG(rmin->key);
            rmin->l = l;
            rmin->update();
            root = rmin;
            root->update();
        }
        // delete n;
    }

    void erase(Key key) {
        node_ptr n = find(key);
        erase(n);
    }

    node_ptr operator [](ssize_t idx) {
        ssize_t sz = idx + 1;
        node_ptr cur = root;
        while (true) {
            DEBUG(cur->key, cur->sz);
            ssize_t lsz = (cur->l ? cur->l->sz : 0);
            ssize_t ord = lsz + 1;
            if (sz == ord) {
                break;
            } else if (sz < ord) {
                cur = cur->l;
            } else {
                sz -= ord;
                cur = cur->r;
            }
        }
        splay(cur);
        return root;
    }
};

}
