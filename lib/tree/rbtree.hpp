#include "../util/template.hpp"

namespace tree {

template <typename T>
struct RedBlackTree {
    struct node {
        using node_ptr = std::shared_ptr<node>;

        node_ptr l, r;
        T dat, minv, maxv;
        bool red;
        ssize_t sz, rank;

        node() { }
        node(node_ptr l, node_ptr r, bool red) : l(l), r(r), red(red) {
            update();
        }
        
        node(T dat) : l(), r(), dat(dat), minv(dat), maxv(dat), red(false), sz(1), rank(0) { }

        void update() {
            if (l.get()) {
                rank = (l.get() ? l->rank + !(l->red) : 0);
                sz = l->sz + r->sz;
                minv = std::min(l->minv, r->minv);
                maxv = std::max(l->maxv, r->maxv);
            } else {
                rank = 0;
                sz = 1;
                minv = dat;
                maxv = dat;
            }
        }
    };

    using node_ptr = typename node::node_ptr;
    using node_pair = std::pair<node_ptr, node_ptr>;
    
    const node_ptr empty = std::shared_ptr<node>();
    node_ptr root = empty;

    RedBlackTree() { }

    RedBlackTree(node_ptr root) : root(root) { }

    node_pair split(node_ptr n, ssize_t sz) {
        if (!sz) return std::make_pair(empty, n);
        if (sz == n->sz) return std::make_pair(n, empty);
        if (sz < n->l->sz) {
            node_ptr fst, snd;
            std::tie(fst, snd) = split(n->l, sz);
            return std::make_pair(fst, merge(snd, n->r));
        } else if (sz > n->l->sz) {
            node_ptr fst, snd;
            std::tie(fst, snd) = split(n->r, sz - n->l->sz);
            return std::make_pair(merge(n->l, fst), snd);
        } else {
            return std::make_pair(n->l, n->r);
        }
    }

    node_ptr merge(node_ptr l, node_ptr r) {
        if (!(l.get())) return r;
        if (!(r.get())) return l;
        node_ptr ret = merge_sub(l, r);
        ret->red = false;
        return ret;
    }

    std::pair<node_ptr, ssize_t> lower_bound(T dat) {
        node_ptr cur = root;
        ll ret = 0;
        while (cur.get() && 1 < cur->sz) {
            auto lmax = cur->l->maxv;
            if (dat <= lmax) {
                cur = cur->l;
            } else {
                if (cur->l.get()) ret += cur->l->sz;
                cur = cur->r;
            }
        }
        if (cur.get()) ret += (cur->dat < dat);
        return std::make_pair(cur, ret);
    }

    std::pair<node_ptr, ssize_t> find(T dat) {
        node_ptr cur = root;
        ll ret = 0;
        while (1 < cur->sz) {
            auto rmin = cur->r->minv;
            if (dat < rmin) {
                cur = cur->l;
            } else {
                if (cur->l.get()) ret += cur->l->sz;
                cur = cur->r;
            }
        }
        if (cur.get() && cur->dat == dat) return std::make_pair(cur, ret);
        else return (empty, -1);
    }

    node_ptr insert(T dat) {
        auto n = std::make_shared<node>(dat);
        if (!root.get()) return root = n;
        auto sz = lower_bound(dat).second;
        node_ptr fst, snd;
        std::tie(fst, snd) = split(root, sz);
        return root = merge(fst, merge(n, snd));
    }

    node_ptr erase_dat(T dat) {
        auto p = find(dat);
        if (!p.first.get()) return root;
        return erase(p.second);
    }

    // 0 indexed
    node_ptr erase(ssize_t sz) {
        node_ptr fst, snd, ret;
        std::tie(fst, snd) = split(root, sz + 1);
        std::tie(fst, ret) = split(fst, sz);
        root = merge(fst, snd);
        return ret;
    }

    void dump_data(node_ptr node) {
        if (node->sz == 1) {
            std::cout << node->dat << ", ";
        } else {
            if (node->l.get()) dump_data(node->l);
            if (node->r.get()) dump_data(node->r);
        }
    }

    void dump_data() {
        dump_data(root);
        std::cout << std::endl;
    }

private:
    
    node_ptr rotate(node_ptr n1, bool clockwise) {
        node_ptr n2 = (clockwise ? n1->l : n1->r);
        if (clockwise) {
            n1->l = n2->r;
            n2->r = n1;
            n1->l->update();
        } else {
            n1->r = n2->l;
            n2->l = n1;
            n1->r->update();
        }
        n1->update();
        n2->update();
        n1->red = true;
        n2->red = false;
        return n2;
    }

    // color of returned node is red.
    // allow red-root and red-child (maybe)
    node_ptr merge_sub(node_ptr l, node_ptr r) {
        if (l->rank == r->rank) return std::make_shared<node>(l, r, true);
        if (l->rank < r->rank) {
            node_ptr n = merge_sub(l, r->l);
            r->l = n;
            r->update();
            if (!(r->red) && n->red && n->l.get() && n->l->red) {
                if (r->r && !(r->r->red)) return rotate(r, true);
                r->red = true;
                r->l->red = false;
                r->r->red = false;
                return r;
            } else {
                return r;
            }
        } else {
            node_ptr n = merge_sub(l->r, r);
            l->r = n;
            l->update();
            if (!(l->red) && n->red && n->r.get() && n->r->red) {
                if (l->l && !(l->l->red)) return rotate(l, false);
                l->red = true;
                l->r->red = false;
                l->l->red = false;
                return l;
            } else {
                return l;
            }
        }
    }
};

}
