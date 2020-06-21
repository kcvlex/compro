#include "../util/template.hpp"
#include "../util/bit-op.hpp"

namespace segtree {

namespace nodes {

template <typename T, typename Comp, T INF>
struct segment {
    static_assert(std::is_integral<T>::value, "T must be integer");

    using size_type = ssize_t;
    T min1, min2; // comp(min1, min2) is true
    T max1, max2; // comp(max2, max1) is true
    T sum, add = 0;
    size_type max_cnt = 0, min_cnt = 0, l, r;

    segment(T min1, T min2, T max1, T max2, T sum, size_type l, size_type r, bool enable_) 
        : min1(min1), min2(min2),
          max1(max1), max2(max2),
          sum(sum), l(l), r(r), enable_(enable_)
    {
    }

    segment(T min1, T min2, T max1, T max2, T sum, size_type l, size_type r)
        : segment(min1, min2, max1, max2, sum, l, r, true)
    {
    }

    segment(T v, size_type l, size_type r)
        : segment(v, INF, v, -INF, v, l, r)
    {
        max_cnt = min_cnt = 1;
    }

    segment() : segment(T(), T(), T(), T(), T(), -1, -1, false) { }

    // caller is chmax
    void update_min(const T &v) noexcept {
        sum -= min1 * min_cnt;
        sum += v * min_cnt;
        if (min1 == max1) {
            min1 = max1 = v;
        } else if (min1 == max2) {
            min1 = max2 = v;
        } else {
            min1 = v;
        }
    }
    
    // caller is chmin
    void update_max(const T &v) noexcept {
        sum -= max1 * max_cnt;
        sum += v * max_cnt;
        if (max1 == min1) {
            max1 = min1 = v;
        } else if (max1 == min2) {
            max1 = min2 = v;
        } else {
            max1 = v;
        }
    }

    void update_add(const T &v) noexcept {
        sum += (r - l) * v;
        min1 += v;
        max1 += v;
        if (min2 != INF) min2 += v;
        if (max2 != -INF) max2 += v;
        add += v;
    }

    bool enable() const noexcept {
        return enable_;
    }

    static segment merge(const segment &s1, const segment &s2) {
        Comp c;
        auto min = [&](const T &a, const T &b) { return c(a, b) ? a : b; };
        auto max = [&](const T &a, const T &b) { return c(a, b) ? b : a; };

        T min1 = min(s1.min1, s2.min1);
        T min2 = (c(s1.min1, s2.min1) ? min(s1.min2, s2.min1) :  // s1.min1 < s2.min1
                  c(s2.min1, s1.min1) ? min(s1.min1, s2.min2) :  // s1.min1 > s2.min1
                                        min(s1.min2, s2.min2));
        
        T max1 = max(s1.max1, s2.max1);
        T max2 = (c(s1.max1, s2.max1) ? max(s1.max1, s2.max2) :  // s1.max1 < s2.max1
                  c(s2.max1, s1.max1) ? max(s1.max2, s2.max1) :  // s1.max1 > s2.max1
                                        max(s1.max2, s2.max2));

        size_type l = std::min(s1.l, s2.l);
        size_type r = std::max(s1.r, s2.r);
        segment ret(min1, min2, max1, max2, s1.sum + s2.sum, l, r);
        if (s1.min1 == ret.min1) ret.min_cnt += s1.min_cnt;
        if (s1.max1 == ret.max1) ret.max_cnt += s1.max_cnt;
        if (s2.min1 == ret.min1) ret.min_cnt += s2.min_cnt;
        if (s2.max1 == ret.max1) ret.max_cnt += s2.max_cnt;
        return ret;
    }

private:
    bool enable_;
};

}

template <typename T, T INF, 
          typename Comp = std::less<T>, 
          typename Segment = nodes::segment<T, Comp, INF>>
struct SegmentTreeBeats {
    using size_type = ssize_t;

private:
    Comp comp;
    vec<Segment> segs;

    // min1 < v < min2
    bool should_update_min(const Segment &s, const T &v) const noexcept {
        return comp(s.min1, v) && comp(v, s.min2);
    }

    // max2 < v < max1
    bool should_update_max(const Segment &s, const T &v) const noexcept {
        return comp(s.max2, v) && comp(v, s.max1);
    }

    void push(size_type idx) {
        auto &s = segs[idx];
        for (int i = 0; i < 2; i++) {
            size_type cidx = 2 * idx + i;
            if (segs.size() <= cidx) break;
            auto &cs = segs[cidx];
            if (!cs.enable()) continue;
            cs.update_add(s.add);
            if (comp(cs.min1, s.min1)) cs.update_min(s.min1);  // cs.min1 < s.min1
            if (comp(s.max1, cs.max1)) cs.update_max(s.max1);  // s.max1 < cs.max1
        }
        s.add = 0;
    }

    void merge(size_type idx) {
        size_type c0 = idx * 2 + 0,
                  c1 = idx * 2 + 1;
        if (segs.size() <= c0 || !segs[c0].enable()) return;
        if (segs.size() <= c1 || !segs[c1].enable()) {
            segs[idx] = segs[c0];
            segs[idx].add = 0;
        } else {
            segs[idx] = Segment::merge(segs[c0], segs[c1]);
        }
    }

    template <typename F>
    void query(size_type ql, size_type qr, F f, size_type idx) {
        if (segs.size() <= idx) return;
        if (!segs[idx].enable()) return;
        push(idx);
        ll nl = segs[idx].l, nr = segs[idx].r;
        if (ql <= nl && nr <= qr) {
            f(idx);
            push(idx);
        } else if (nr <= ql || qr <= nl) {
            // nothing
        } else {
            query(ql, qr, f, 2 * idx + 0);
            query(ql, qr, f, 2 * idx + 1);
            merge(idx);
        }
    }

    void chmin(size_type ql, size_type qr, size_type idx, const T &t) {
        if (segs.size() <= idx) return;
        auto &s = segs[idx];
        if (!s.enable()) return;
        push(idx);
        ll nl = s.l, nr = s.r;
        if (nr <= ql || qr <= nl) return;
        if (comp(s.max1, t) || s.max1 == t) return; // max1 <= t
        if (ql <= nl && nr <= qr && should_update_max(s, t)) {
            s.update_max(t);
            push(idx);
        } else {
            chmin(ql, qr, 2 * idx + 0, t);
            chmin(ql, qr, 2 * idx + 1, t);
            merge(idx);
        }
    }

    void chmax(size_type ql, size_type qr, size_type idx, const T &t) {
        if (segs.size() <= idx) return;
        auto &s = segs[idx];
        if (!s.enable()) return;
        push(idx);
        ll nl = s.l, nr = s.r;
        if (nr <= ql || qr <= nl) return;
        if (comp(t, s.min1) || s.min1 == t) return; // t <= s.min1
        if (ql <= nl && nr <= qr && should_update_min(s, t)) {
            s.update_min(t);
            push(idx);
        } else {
            chmax(ql, qr, 2 * idx + 0, t);
            chmax(ql, qr, 2 * idx + 1, t);
            merge(idx);
        }
    }

public:
    template <typename F>
    SegmentTreeBeats(F f, size_type sz) : segs(2 * utility::ceil_pow2(sz)) {
        for (size_type i = 0; i < sz; i++) {
            segs[segs.size() / 2 + i] = Segment(f(i), i, i + 1);
        }
        for (size_type i = segs.size() / 2 - 1; 1 <= i; i--) {
            merge(i);
        }
    }

    size_type size() const noexcept {
        return segs.size() / 2;
    }

    void chmin(size_type ql, size_type qr, const T &t) {
        chmin(ql, qr, 1, t);
    }

    void chmax(size_type ql, size_type qr, const T &t) {
        chmax(ql, qr, 1, t);
    }

    void add(size_type ql, size_type qr, const T &t) {
        auto f = [&](size_type i) { 
            segs[i].update_add(t); 
        };
        query(ql, qr, f, 1);
    }

    T get_sum(size_type ql, size_type qr) {
        T ret = T();
        auto f = [&](size_type i) {
            ret += segs[i].sum; 
        };
        query(ql, qr, f, 1);
        return ret;
    }

    T get_min(size_type ql, size_type qr) {
        T ret = INF;
        auto f = [&](size_type i) {
            auto v = segs[i].min1;
            if (comp(v, ret)) ret = v;
        };
        query(ql, qr, f, 1);
        return ret;
    }

    T get_max(size_type ql, size_type qr) {
        T ret = -INF;
        auto f = [&](size_type i) {
            auto v = segs[i].max1;
            if (comp(ret, v)) ret = v;
        };
        query(ql, qr, f, 1);
        return ret;
    }
};

}
