#pragma once
#include "util/template.hpp"
#include "util/debug.hpp"

namespace tree {
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
void rotate_aux(Node *n, bool cc) {
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
        if (a) a->p = p;
        if (b) b->p = p;
        if (c) c->p = x;
    } else {
        a = x->l; b = x->r; c = p->r;
        x->l = a; x->r = p; x->p = pp;
        p->l = b; p->r = c; p->p = x;
        if (pp && !rp) {
            if (ppl) pp->l = x;
            else pp->r = x;
        }
        if (a) a->p = x;
        if (b) b->p = p;
        if (c) c->p = p;
    }
    p->update();
    x->update();
    if (pp) pp->update();  // is this necessary??
}

template <typename Node>
void zig(Node *n) {
    n->rotate(!is_l(n));
}

template <typename Node>
void zig_zig(Node *n) {
    bool wise = !is_l(n);
    n->p->rotate(wise);
    n->rotate(wise);
}

template <typename Node>
void zig_zag(Node *n) {
    bool wise = !is_l(n);
    n->rotate(wise);
    n->rotate(!wise);
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

}
