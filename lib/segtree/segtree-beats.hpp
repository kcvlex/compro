#include "../util/template.hpp"
#include "../util/bit-op.hpp"
#include "../util/monoid-validator.hpp"
#include "/home/taroy/kyopuro/lib/util/debug.hpp"

namespace segtree {

namespace nodes {

template <typename T, typename Comp>
struct segment {
    using size_type = ssize_t;
    T min1, min2, min_lazy; // comp(min1, min2) is true
    T max1, max2, max_lazy; // comp(max2, max1) is true
    T sum;
    size_type max_cnt = 0, min_cnt = 0;
    bool has_max_lazy = false, has_min_lazy = false;

    segment(T min1, T min2, T max1, T max2, T sum) 
        : min1(min1), min2(min2), min_lazy(T()), 
          max1(max1), max2(max2), max_lazy(T()), sum(sum) { }

    segment() : segment(T(), T(), T(), T()) { }

    void update_lazy_max(const T &v) noexcept {
        apply();
        if (comp(max1, v)) return;
        has_max_lazy = true;
        max_lazy = v;
    }

    void update_lazy_min(const T & v) noexcept {
        apply();
        if (comp(v, min1)) return;
        has_min_lazy = true;
        min_lazy = v;
    }

    void apply() noexcept {
        apply_min();
        apply_max();
        update_minmax();
    }

    static segment merge(const segment &s1, const segment &s2) {
        constexpr size_type sz = 4;
        T min_lis[] = { s1.min1, s1.min2, s2.min1, s2.min2, };
        T max_lis[] = { s1.max1, s1.max2, s2.max1, s2.max2, };
        Comp c;
        std::sort(min_lis, min_lis + 4, c);
        std::unique(min_lis, min_lis + 4);
        std::sort(max_lis, max_lis + 4, [&](const T &a, const T &b) { return !c(a, b); });
        std::unique(max_lis, max_lis + 4);
        segment ret(min_lis[0], min_lis[1], max_lis[0], max_lis[1], s1.sum + s2.sum);
        for (auto ptr : { &s1, &s2 }) {
            if (ptr->min1 == ret.min1) ret.min_cnt += ptr->min_cnt;
            if (ptr->max1 == ret.max1) ret.max_cnt += ptr->max_cnt;
        }
        return ret;
    }

private:
    // min1 < min_lazy < min2
    void apply_min() noexcept {
        if (!has_lazy_min) return;
        has_lazy_min = false;
        sum += lazy_min * min_cnt;
        sum -= min1 * min_cnt;
        min1 = lazy_min;
    }

    // max2 < maz_lazy < max1
    void apply_max() noexcept {
        if (!has_lazy_max) return;
        has_lazy_max = false;
        sum += lazy_max * max_cnt;
        sum -= max1 * max_cnt;
        max1 = lazy_max;
    }

    void update_minmax() {
        T lis[] = { min1, min2, max1, max2, };
        std::sort(min1, min2, max1, max2, Comp());
        auto end = std::unique(lis, lis + 4);
        min1 = lis[0]; min2 = lis[1];
        max1 = *(end - 1), max2 = *(end - 2);
    }
};

}

template <typename T, typename Comp = std::less<T>, 
          typename Segment = nodes::segment<T, Comp>>
struct SegmentTreeBeats {
    using size_type = ssize_t;

private:
    Comp comp;
    T id_ele, inf;
    vec<Segment> segs;
    size_type height;

    void push(size_type idx) {
        auto &s = segs[idx];
        if (!s.has_min_lazy && !s.has_max_lazy) return;
        auto min_lazy = s.min_lazy, max_lazy = s.max_lazy;
        bool has_min_lazy = s.has_min_lazy, has_max_lazy = s.has_max_lazy;
        s.apply();
        for (int i = 0; i < 2; i++) {
            size_type cidx = idx + i;
            if (segs.size() <= cidx) break;
            auto &cs = segs[cidx];
            if (has_min_lazy) cs.update_lazy_min(min_lazy);
            if (has_max_lazy) cs.update_lazy_max(max_lazy);
        }
    }

    void update_from_bottom(size_type idx) {
        while (true) {
            auto pidx = idx / 2;
            if (pidx == 0) break;
            auto c1 = 2 * pidx + 0;
            auto c2 = 2 * pidx + 1;
            push(c1); push(c2);
            segs[idx] = merge(segs[c1], segs[c2]);
            idx = pidx;
        }
    }

    void propagate_from_top(size_type idx) {
        for (int i = height; 1 <= i; i--) push(idx >> i);
    }

    size_type get_endpoint_seg_idx(size_type i) const noexcept {
        i += size();
        return i / (i & -i);
    }

    auto query_preprocess(size_type ql, size_type qr) -> std::pair<size_type, size_type> {
        for (auto i : { ql, qr }) {
            auto idx = get_endpoint_seg_idx(i);
            propagate_from_top(i);
        }
        return std::make_pair(ql + size(), qr + size());
    }

    void update_query_postprocess(size_type ql, size_type qr) {
        for (auto i : { ql, qr }) {
            auto idx = get_endpoint_seg_idx(i);
            update_from_bottom(i);
        }
    }

    template <typename F>
    void traverse_necessary_segs(size_type ql, size_type qr, F f) {
        auto [ lnode, rnode ] = query_preprocess(ql, qr);
        while (lnode < rnode) {
            if (lnode & 1) {
                push(lnode);
                f(lnode);
                lnode++;
            }
            if (rnode & 1) {
                rnode--;
                push(rnode);
                f(rnode);
            }
            lnode /= 2, rnode /= 2;
        }
    }

public:
    template <typename F>
    SegmentTreeBeats(F f, size_type sz, T id_ele, T inf)
        : id_ele(id_ele), inf(inf),
          segs(2 * utility::ceil_pow2(sz)),
          height(utility::msb(segs.size() / 2))
    {
        for (size_type i = 0; i < sz; i++) segs[i + sz] = Segment(f(i), id_ele);
        for (size_type i = sz - 1; 1 <= i; i--) {
            size_type c1 = 2 * i + 0;
            size_type c2 = 2 * i + 1;
            segs[i] = Segment::merge(segs[c1], segs[c2]);
        }
    }

    size_type size() const noexcept {
        return segs.size() / 2;
    }

    void chmin(size_type ql, size_type qr, const T &t) {
        auto f = [&](size_type i) {
            auto &s = segs[i];
            bool update = comp(s.max2, t) && comp(t, s.max1);  // max2 < t < max1
            if (!update) return;
            s.update_max(t);
        };
        traverse_necessary_segs(ql, qr, f);
        update_query_postprocess(ql, qr);
    }

    void chmax(size_type ql, size_type qr, const T &t) {
        auto f = [&](size_type i) {
            auto &s = segs[i];
            bool update = comp(s.min1, t) && comp(t, s.min2);  // min1 < t < min2
            if (!update) return;
            s.update_min(t);
        };
        traverse_necessary_segs(ql, qr, f);
        update_query_postprocess(ql, qr);
    }

    T get_sum_query(size_type ql, size_type qr) {
        T ret = T();
        auto f = [&](size_type i) { retl += segs[i].sum; };
        traverse_necessary_segs(ql, qr, f);
    }

    T get_min(size_type ql, size_type qr) {
        T ret = inf;
        auto f = [&](size_type i) {
            auto v = segs[i].min1;
            if (comp(v, ret)) ret = v;
        };
        traverse_necessary_segs(ql, qr, f);
        return ret;
    }

    T get_max(size_type ql, size_type qr) {
        T ret = -inf;
        auto f = [&](size_type i) {
            auto v = segs[i].max1;
            if (comp(ret, v)) ret = v;
        };
        traverse_necessary_segs(ql, qr, f);
        return ret;
    }
};
