#include "../util/template.cpp"

namespace segtree {

template <typename T>
struct SegmentTree {
    using Merge = std::function<T(T, T)>;
    vec<T> nodes;
    Merge merge_f;
    T id_ele;

    SegmentTree(const vec<T> &init_v, Merge merge_f, T id_ele) 
        : nodes(ceil_pow2(init_v.size()) * 2 - 1, id_ele), merge_f(merge_f), id_ele(id_ele)
    {
        ssize_t s = size();
        for (ssize_t i = 0; i < init_v.size(); i++) nodes[i + s - 1] = init_v[i];
        for (ssize_t i = s - 2; 0 <= i; i--) nodes[i] = get_merged(i);
    }

    size_t size() const {
        return (nodes.size() + 1) / 2;
    }

    const T& operator [](ll idx) const {
        return nodes[idx + size() - 1];
    }

    T get_query(ll ql, ll qr, ll nl, ll nr, ll nidx) const {
        if (nr <= ql || qr <= nl) return id_ele;
        if (ql <= nl && nr <= qr) return nodes[nidx];
        ll mid = (nl + nr) / 2;
        ll lidx, ridx;
        std::tie(lidx, ridx) = get_children_idx(nidx);
        auto lv = get_query(ql, qr, nl, mid, lidx);
        auto rv = get_query(ql, qr, mid, nr, ridx);
        return merge_f(lv, rv);
    }

    T get_query(ll ql, ll qr) const {
        return get_query(ql, qr, 0, size(), 0);
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

    // FIXME : test
    T lower_bound(ll idx, std::function<bool(T)> check, T sum) const {
        if (size() - 1 <= idx) return idx - (size() - 1);
        ll lidx, ridx;
        std::tie(lidx, ridx) = get_children_idx(idx);
        auto lv = merge_f(sum, nodes[lidx]);
        if (check(lv)) return lower_bound(lidx, check, sum);
        else return lower_bound(ridx, check, lv);
    }

    T lower_bound(std::function<bool(T)> check) const {
        return lower_bound(0, check, id_ele);
    }

private:
    ssize_t ceil_pow2(ssize_t s) {
        ssize_t ret = 1;
        while (ret <= s) ret *= 2;
        return ret;
    }

    pll get_children_idx(ll idx) const {
        return pll(2 * idx + 1, 2 * idx + 2);
    }

    T get_merged(ll idx) const {
        ll a, b;
        std::tie(a, b) = get_children_idx(idx);
        return merge_f(nodes[a], nodes[b]);
    }
};

}
