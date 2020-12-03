#pragma once
#include "util/template.hpp"
#include "util/debug.hpp"

namespace tree {

namespace lctree {

template <typename AbelianGroup>
struct subtree_link_cut_node {
    using node_ptr = subtree_link_cut_node<AbelianGroup>*;
    using value_type = AbelianGroup;

    node_ptr p, ch[2];
    size_type sz, sz_light;  // sz = 1 + sz_light + l->sz + r->sz
    value_type val, sum, sum_light, add, done;  // sum = val + sum_light + l->sum + r->sum
    bool rev;  // children should be reversed or not

    subtree_link_cut_node(node_ptr p, node_ptr l, node_ptr r, AbelianGroup val)
        : p(p), sz(1), sz_light(0),
          val(val), sum(val), sum_light(), add(), done(), 
          rev(false)
    {
        ch[0] = l;
        ch[1] = r;
    }

    subtree_link_cut_node() 
        : subtree_link_cut_node(nullptr, nullptr, nullptr, value_type())
    {
    }

    bool is_root() const {
        return !p || (p->ch[0] != this && p->ch[1] != this);
    }

    void update() {
        push();
        sz = 1 + sz_light;
        sum = val + sum_light;
        for (int i = 0; i < 2; i++) {
            if (!ch[i]) continue;
            ch[i]->push();
            sz += ch[i]->sz;
            sum += ch[i]->sum;
        }
    }

    void prop_from_parent(value_type v) {
        value_type delta = v + done.inv();
        add_all(delta);
        done = v;
    }

    void push() {
        if (p) prop_from_parent(p->add);
        if (rev) {
            std::swap(ch[0], ch[1]);
            if (ch[0]) ch[0]->rev ^= true;
            if (ch[1]) ch[1]->rev ^= true;
        }
        /*
        for (int i = 0; i < 2; i++) {
            if (!ch[i]) continue;
            ch[i]->prop_from_parent(add);
            if (rev) ch[i]->toggle();
        }
        */
        rev = false;
    }

    void toggle() {
        std::swap(ch[0], ch[1]);
        rev ^= true;
    }

    void rotate(bool cc) {
        cc ^= true;
        auto np = ch[cc];
        assert(np);
        if (np->ch[!cc]) np->ch[!cc]->push();
        ch[cc] = np->ch[!cc];
        if (ch[cc]) {
            ch[cc]->p = this;
            ch[cc]->done = add;
        }
        if (p) {
            if (p->ch[0] == this) p->ch[0] = np;
            if (p->ch[1] == this) p->ch[1] = np;
        }
        np->p = p;
        np->done = (p ? p->add : value_type());
        p = np;
        np->ch[!cc] = this;
        done = p->add;
        update(); np->update();
    }

    void splay() {
        while (!is_root()) {
            if (p->is_root()) {
                p->push(); push();
                p->rotate(p->ch[0] == this);
            } else {
                auto pp = p->p;
                pp->push(); p->push(); push();
                bool a = (pp->ch[0] == p);
                bool b = (p->ch[0] == this);
                assert(pp->ch[0] == p || pp->ch[1] == p);
                assert(p->ch[0] == this || p->ch[1] == this);
                if (a == b) pp->rotate(a);
                p->rotate(b);
                if (a != b) pp->rotate(a);
            }
        }
        push();
    }

    void light2heavy() {
        if (!p) return;
        p->sum_light += sum.inv();
        p->sz_light -= sz;
    }

    void heavy2light() {
        if (!p) return;
        p->sum_light += sum;
        p->sz_light += sz;
    }

    void add_all(value_type delta) {
        val += delta;
        add += delta;
        sum += delta.pow(sz);
        sum_light += delta.pow(sz_light);
    }
};

template <typename AbelianGroup>
struct SubtreeLinkCutTree {
    using node_type = subtree_link_cut_node<AbelianGroup>;
    using node_ptr = node_type*;
    using value_type = typename node_type::value_type;

    // https://stackoverflow.com/questions/5410035/when-does-a-stdvector-reallocate-its-memory-array
    template <typename F>
    SubtreeLinkCutTree(F f, size_type sz) {
        nodes.reserve(sz);
        for (size_type i = 0; i < sz; i++) {
            nodes.emplace_back(nullptr, nullptr, nullptr, f(i));
        }
    }

    node_ptr expose(size_type idx) {
        return expose(&nodes[idx]);
    }

    node_ptr expose(node_ptr n) {
        node_ptr r = nullptr;
        for (node_ptr cur = n; cur; cur = cur->p) {
            cur->splay();
            auto pr = cur->ch[1], nr = r;
            
            cur->push();
            if (pr) {
                pr->push();
                pr->heavy2light();
            }
            
            cur->ch[1] = nr;
            cur->push();
            if (nr) {
                nr->push();
                nr->light2heavy();
            }
            
            cur->update();
            r = cur;
        }
        n->splay();
        return n;
    }

    void link(size_type p, size_type c) {
        evert(c);
        auto cp = expose(c);
        auto pp = expose(p);  // pp->r is nullptr
        cp->push();
        cp->done = pp->add;
        cp->p = pp;
        pp->ch[1] = cp;
        pp->push();
        pp->update();
    }

    // node and its (original tree's) parent.
    void cut(size_type idx) {
        auto p = expose(idx);
        auto lp = p->ch[0];
        p->push();
        lp->push();
        p->ch[0] = nullptr;
        lp->p = nullptr;
        p->update();
        lp->done = value_type();
    }

    void evert(size_type idx) {
        auto p = expose(idx);
        //p->toggle();
        p->rev ^= true;
        p->push();
    }

    void add_vertex(size_type idx, value_type delta) {
        auto p = expose(idx);
        p->val += delta;
        p->sum += delta;
    }

    value_type subtree_sum(size_type p, size_type v) {
        evert(p);
        cut(v);
        auto ret = expose(v)->sum;
        link(p, v);
        return ret;
    }

    void add_subtree(size_type p, size_type v, value_type delta) {
        evert(p);
        cut(v);
        auto n = expose(v);
        n->add_all(delta);
        link(p, v);
    }

private:
    vec<node_type> nodes;
};

}

}
