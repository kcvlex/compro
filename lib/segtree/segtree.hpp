#include "util/template.hpp"
#include "util/debug.hpp"
#include "util/bit-op.hpp"

namespace segtree {

template <typename M>
struct SegmentTree {
    vec<M> nodes;

    auto get_children_idx(size_type idx) {
        return std::make_pair(2 * idx, 2 * idx + 1);
    }

    void update_node(size_type i) {
        auto [ a, b ] = get_children_idx(i);
        nodes[i] = M::merge(nodes[a], nodes[b]);
    }

public:
    template <typename F>
    SegmentTree(F f, size_type sz) {
        build(f, sz);
    }

    template <typename F>
    void build(F f, size_type sz) {
        nodes.resize(ceil_pow2(sz) * 2);
        auto offset = size();
        for (size_type i = 0; i < sz; i++) {
            nodes[i + offset] = f(i);
        }
        for (size_type i = offset - 1; 1 <= i; i--) {
            update_node(i);
        }
    }

    size_type size() const {
        return nodes.size() / 2;
    }

    const M& operator [](size_type i) const {
        return nodes[size() + i];
    }

    M get_query(ll ql, ll qr) const {
        M ret;
        size_type lnode = ql + size(), rnode = qr + size();
        while (lnode < rnode) {
            if (lnode & 1) {
                DEBUG(nodes[lnode].v);
                ret = M::merge(nodes[lnode], ret);
                lnode++;
            }
            if (rnode & 1) {
                rnode--;
                DEBUG(nodes[rnode].v);
                ret = M::merge(ret, nodes[rnode]);
            }
            lnode /= 2;
            rnode /= 2;
        }
        return ret;
    }

    void update_query(ll idx, M val) {
        idx += size();
        nodes[idx] = val;
        while (1 < idx) {
            ll pidx = idx / 2;
            update_node(pidx);
            idx = pidx;
        }
    }

    template <typename F>
    size_type lower_bound(F f) const {
        M sum;
        size_type l = 0, r = size(), idx = 0, bidx = 0;
        if (!f(nodes[0])) return -1;
        while (true) {
            if (r - l == 1) {
                if (!f(sum)) bidx++;
                break;
            }
            auto lidx = 2 * idx;
            auto ridx = lidx + 1;
            auto nsum = M::merge(sum, nodes[lidx]);
            size_type m = (l + r) / 2;
            if (f(nsum)) {
                r = m;
                idx = lidx;
            } else {
                l = m;
                idx = ridx;
                bidx = m;
                sum = std::move(nsum);
            }
        }
        return bidx;
    }
};

}
