#pragma once
#include "../splay-tree.hpp"
#include "../../util/monoid-validator.hpp"
#include "/home/taroy/kyopuro/lib/util/debug.hpp"

namespace tree {

namespace lctree {

template <typename M, typename Op>
struct link_cut_node {
    using node_ptr = link_cut_node<M, Op>*;
    using size_type = ssize_t;
    node_ptr l, r, p;
    size_type sz;
    M m, sum = M(), rsum = M();
    Op op = Op();
    bool rev, lazy;

    link_cut_node(node_ptr p, node_ptr l, node_ptr r, M m) 
        : p(p), l(l), r(r), sz(1), m(m), rev(false), lazy(false) { }
    link_cut_node() 
        : link_cut_node(nullptr, nullptr, nullptr, M()) { }

    bool is_root() const {
        return !p || (p->l != this && p->r != this);
    }

    void init_lazy() {
        lazy = false;
        op = Op();
    }

    void update() {
        sz = 1;
        sum = m;
        rsum = m;
        if (l) {
            sum = M::merge(l->sum, sum);
            rsum = M::merge(rsum, l->rsum);
            sz += l->sz;
            l->p = this;
        }
        if (r) {
            sum = M::merge(sum, r->sum);
            rsum = M::merge(r->rsum, rsum);
            sz += r->sz;
            r->p = this;
        }
    }
};

template <typename M, typename Op>
void propagate(link_cut_node<M, Op> *n, const Op op) {
    n->lazy = true;
    n->op = Op::merge(n->op, op);
    n->m.apply(op);
    n->sum.apply(op);
    n->rsum.apply(op);
}

template <typename M, typename Op>
void toggle(link_cut_node<M, Op> *n) {
    std::swap(n->l, n->r);
    n->update();
    n->rev ^= true;
}

template <typename M, typename Op>
void push(link_cut_node<M, Op> *n) {
    if (n->lazy) {
        for (auto c : { n->l, n->r }) if (c) propagate(c, n->op);
        n->init_lazy();
    }
    if (n->rev) {
        if (n->l) toggle(n->l);
        if (n->r) toggle(n->r);
        n->rev = false;
    }
}

template <typename M, typename Op>
void splay(link_cut_node<M, Op> *n) {
    push(n);
    while (!n->is_root()) {
        auto p = n->p;
        assert(p != p->p);
        assert(p != p->l);
        assert(p != p->r);
        if (p->p) push(p->p);
        push(p);
        push(n);
        tree::splay_aux(n);
        assert(p != p->p);
        assert(p != p->l);
        assert(p != p->r);
    }
}

template <typename M, typename Op>
struct LinkCutTree {
    using node_type = link_cut_node<M, Op>;
    using node_ptr = node_type*;
    using size_type = typename node_type::size_type;

    template <typename F>
    LinkCutTree(F f, size_type sz) : nodes(sz) {
        for (size_type i = 0; i < sz; i++) {
             nodes[i] = new node_type(nullptr, nullptr, nullptr, f(i));
        }
    }

    node_ptr expose(size_type idx) {
        node_ptr r = nullptr;
        for (node_ptr cur = nodes[idx]; cur; cur = cur->p) {
            splay(cur);
            cur->r = r;
            cur->update();
            r = cur;
        }
        auto ret = nodes[idx];
        splay(ret);
        return ret;
    }

    void link(size_type p, size_type c) {
        evert(c);
        auto cp = expose(c);
        auto pp = expose(p);
        cp->p = pp;
        pp->r = cp;
        pp->update();
    }

    void cut(size_type idx) {
        auto p = expose(idx);
        auto lp = p->l;
        p->l = nullptr;
        p->update();
        lp->p = nullptr;
    }

    void evert(size_type idx) {
        auto p = expose(idx);
        toggle(p);
        push(p);
    }

    void apply_path(size_type idx, Op op) {
        auto p = expose(idx);
        propagate(p, op);
        push(p);
    }

    void apply(size_type idx, Op op) {
        auto p = expose(idx);
        p->m.apply(op);
        p->update();
        push(p);
    }

private:
    vec<node_ptr> nodes;
};

}

}
