#include "../util/template.hpp"
#include "../util/ceil-pow2.hpp"

namespace segtree {

template <typename T, typename Op>
struct SegmentTree {
    vec<T> nodes;
    Op op;
    T id_ele;

    auto get_children_idx(ssize_t idx) const -> std::pair<ssize_t, ssize_t> {
        return std::make_pair(2 * idx + 1, 2 * idx + 2);
    }

    T get_merged(ssize_t idx) const {
        ssize_t a, b;
        std::tie(a, b) = get_children_idx(idx);
        return op(nodes[a], nodes[b]);
    }

public:
    SegmentTree(const vec<T> &init_v, Op op, T id_ele)
        : op(op), id_ele(id_ele)
    {
        build(init_v);
    }

    void build(const vec<T> &v) {
        nodes.resize(ceil_pow2(v.size()) * 2 - 1, id_ele);
        ssize_t sz = size();
        std::copy(ALL(v), nodes.begin() + sz - 1);
        build_parents();
    }

    void build_parents() {
        ssize_t sz = size();
        for (ssize_t i = sz - 2; 0 <= i; i--) nodes[i] = get_merged(i);
    }

    ssize_t size() const {
        return (nodes.size() + 1) / 2;
    }

    const T& operator [](ll idx) const {
        return nodes[idx + size() - 1];
    }

    T get_query(ll ql, ll qr) const {
        return get_query(ql, qr, id_ele);
    }

    T get_query(ll ql, ll qr, T init) const {
        T ret = init;
        ssize_t lnode = ql + size(), rnode = qr + size();
        while (lnode < rnode) {
            if (lnode & 1) {
                ret = std::move(op(nodes[lnode - 1], ret));
                lnode++;
            }
            if (rnode & 1) {
                rnode--;
                ret = std::move(op(ret, nodes[rnode - 1]));
            }
            lnode /= 2;
            rnode /= 2;
        }
        return ret;
    }

    void update_query(ll idx, T val) {
        idx += size() - 1;
        nodes[idx] = val;
        while (idx) {
            ll pidx = (idx - 1) / 2;
            nodes[pidx] = get_merged(pidx);
            idx = pidx;
        }
    }

    template <typename F>
    ssize_t lower_bound(F f) const {
        T sum = id_ele;
        ssize_t l = 0, r = size(), idx = 0, bidx = 0;
        if (!f(nodes[0])) return -1;
        while (true) {
            if (r - l == 1) {
                if (!f(sum)) bidx++;
                break;
            }
            auto lidx = 2 * idx + 1;
            auto ridx = lidx + 1;
            T nsum = op(sum, nodes[lidx]);
            ssize_t m = (l + r) / 2;
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
