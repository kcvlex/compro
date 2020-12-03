#pragma once
#include "tree/bbst-base.hpp"

namespace tree {

namespace lctree {

template <typename M, typename Op>
struct path_link_cut_node {
    using node_type = path_link_cut_node<M, Op>;
    using node_ptr = node_type*;
    using value_type = M;
    using op_type = Op;
    
    node_ptr p, l, r;
    size_type sz;
    value_type m, sum, rsum;
    Op op = Op();
    bool rev, lazy;

    path_link_cut_node(node_ptr p, node_ptr l, node_ptr r, M m) 
        : p(p), l(l), r(r), sz(1), m(m), sum(m), rsum(m), rev(false), lazy(false)
    {
    }
    
    path_link_cut_node() 
        : path_link_cut_node(nullptr, nullptr, nullptr, M())
    {
    }

    bool is_root() const {
        return !p || (p->l != this && p->r != this);
    }

    void update() {
        sz = 1;
        sum = m;
        rsum = m;

        if (l) {
            if (!rev) {
                sum = std::move(value_type::merge(l->sum, sum));
                rsum = std::move(value_type::merge(rsum, l->rsum));
            } else {
                sum = std::move(value_type::merge(l->rsum, sum));
                rsum = std::move(value_type::merge(rsum, l->sum));
            }
            sz += l->sz;
            l->p = this;
        }

        if (r) {
            if (!rev) {
                sum = std::move(value_type::merge(sum, r->sum));
                rsum = std::move(value_type::merge(r->rsum, rsum));
            } else {
                sum = std::move(value_type::merge(sum, r->rsum));
                rsum = std::move(value_type::merge(r->sum, rsum));
            }
            sz += r->sz;
            r->p = this;
        }
    }

    void propagate(const Op &prop) {
        lazy = true;
        op = std::move(op_type::merge(op, prop));
        m.apply(prop);
        sum.apply(prop);
        rsum.apply(prop);
    }

    void toggle() {
        std::swap(l, r);
        rev ^= true;
        update();
    }

    void push() {
        auto push_aux = [&](node_ptr n) {
            if (lazy) n->propagate(op);
            if (rev) n->toggle();
        };

        if (l) push_aux(l);
        if (r) push_aux(r);
    
        lazy = false;
        rev = false;
        op = op_type();
    }

    void rotate(bool cc) {
        tree::rotate_aux(this, cc);
    }

    void splay() {
        push();
        while (!is_root()) {
            if (p->p) p->p->push();
            p->push();
            push();
            tree::splay_aux(this);
        }
    }
};

template <typename M, typename Op>
struct PathLinkCutTree {
    using node_type = path_link_cut_node<M, Op>;
    using node_ptr = node_type*;
    using value_type = typename node_type::value_type;

    // https://stackoverflow.com/questions/5410035/when-does-a-stdvector-reallocate-its-memory-array
    template <typename F>
    PathLinkCutTree(F f, size_type sz) {
        nodes.reserve(sz);
        for (size_type i = 0; i < sz; i++) {
             nodes.emplace_back(nullptr, nullptr, nullptr, f(i));
        }
    }

    node_ptr expose(size_type idx) {
        node_ptr r = nullptr;
        for (node_ptr cur = &nodes[idx]; cur; cur = cur->p) {
            cur->splay();
            cur->r = r;
            cur->update();
            r = cur;
        }
        auto ret = &nodes[idx];
        ret->splay();
        return ret;
    }

    void link(size_type p, size_type c) {
        evert(c);
        auto cp = expose(c);
        auto pp = expose(p);
        rp->p = pp;
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
        p->toggle();
        p->push();
    }

    void apply_path(size_type idx, const Op &op) {
        auto p = expose(idx);
        p->propagate(op);
        p->push();
    }

    void apply(size_type idx, const Op &op) {
        auto p = expose(idx);
        p->push();
        p->m.apply(op);
        p->update();
    }

    value_type get_value(size_type idx) {
        expose(idx)->push();
        return nodes[idx].m;
    }

    // nodes[src].m + ... + nodes[sink].m
    value_type get_path_value(size_type src, size_type sink) {
        evert(src);
        auto p = expose(sink);
        return p->sum;
    }

private:
    vec<node_type> nodes;
};

}

}
