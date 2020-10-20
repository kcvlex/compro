#include "../util/template.hpp"
#include "../util/bit-op.hpp"

namespace segtree {

template <typename M, typename Op>
class LazySegmentTree {
    struct segment {
        M m;
        Op op;
        bool has_lazy;

        segment(M m = M()) : m(m), op(Op()), has_lazy(false) { }

        void update_op(Op o) {
            m.apply(o);
            op = Op::merge(op, o);
            has_lazy = true;
        }

        void init_op() {
            op = Op();
            has_lazy = false;
        }
    };

    vec<segment> segs;
    size_type height;

    void push(size_type idx) {
        auto &s = segs[idx];
        if (!s.has_lazy) return;
        for (int i = 0; i < 2; i++) {
            auto cidx = 2 * idx + i;
            if (segs.size() <= cidx) break;
            auto &cs = segs[cidx];
            cs.update_op(s.op);
        }
        s.init_op();
    }

    void propagate_from_top(size_type idx) {
        for (int i = height; 1 <= i; i--) push(idx >> i);
    }

    void update_from_bottom(size_type idx) {
        while (true) {
            auto pidx = idx / 2;
            if (pidx == 0) break;
            size_type c0 = 2 * pidx + 0,
                      c1 = 2 * pidx + 1;
            push(c0); push(c1);
            segs[pidx].m = M::merge(segs[c0].m, segs[c1].m);
            idx = pidx;
        }
    }

    size_type get_endpoint_seg(size_type i) {
        i += size();
        return i / (i & -i);
    }

public:
    template <typename F>
    LazySegmentTree(F f, size_type sz) {
        size_type sz2 = ceil_pow2(sz);
        segs.resize(sz2 * 2);
        height = utility::msb(sz2);
        for (size_type i = 0; i < sz; i++) segs[i + sz2] = f(i);
        for (size_type i = sz2 - 1; 1 <= i; i--) segs[i] = M::merge(segs[2 * i].m, segs[2 * i + 1].m);
    }

    template <typename T>
    LazySegmentTree(const vec<T> &v) 
        : LazySegmentTree([&](size_type i) { return v[i]; }, v.size()) { }

    size_type size() const {
        return segs.size() / 2;
    }

    template <typename T>
    void update_query(size_type ql, size_type qr, const T &t) {
        Op op(t);
        auto l0 = get_endpoint_seg(ql);
        auto r0 = get_endpoint_seg(qr);
        propagate_from_top(l0);
        propagate_from_top(r0);
        size_type lnode = ql + size(), rnode = qr + size();
        while (lnode < rnode) {
            if (lnode & 1) {
                segs[lnode].update_op(op);
                push(lnode);
                lnode++;
            }
            if (rnode & 1) {
                rnode--;
                segs[rnode].update_op(op);
                push(rnode);
            }
            lnode /= 2;
            rnode /= 2;
        }
        update_from_bottom(l0);
        update_from_bottom(r0);
    }

    M get_query(ll ql, ll qr) {
        auto ret = M();
        auto l0 = get_endpoint_seg(ql);
        auto r0 = get_endpoint_seg(qr);
        propagate_from_top(l0);
        propagate_from_top(r0);
        size_type lnode = ql + size(), rnode = qr + size();
        while (lnode < rnode) {
            if (lnode & 1) {
                push(lnode);
                ret = M::merge(segs[lnode].m, ret);
                lnode++;
            }
            if (rnode & 1) {
                rnode--;
                push(rnode);
                ret = M::merge(ret, segs[rnode].m);
            }
            lnode /= 2;
            rnode /= 2;
        }
        return ret;
    }
};

}
