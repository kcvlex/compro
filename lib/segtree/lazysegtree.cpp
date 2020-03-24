#include "../util/template.cpp"

namespace segtree {

template <typename T, typename L>
class LazySegmentTree {
    using merge_t = std::function<T(T, T)>;
    using apply_t = std::function<T(T, L)>;
    using update_t = std::function<L(L, L)>;
    using calc_lazy_t = std::function<L(ssize_t, ssize_t, L)>;
    using prop_t = std::function<L(L)>;
    
    T id_val;
    L id_lazy;

    struct segment {
        T val;
        std::pair<L, bool> lazy;
        ll l, r, idx, cl, cr;

        segment(const T &val, const std::pair<L, bool> &lazy, ll l, ll r, ll idx)
            : val(val), lazy(lazy), l(l), r(r)
        {
            set_idx(idx);
        }

        void set_idx(ll idx) {
            this->idx = idx;
            cl = 2 * idx + 1;
            cr = 2 * idx + 2;
        }

        bool is_lazy() const {
            return lazy.second;
        }

        bool is_leaf() const {
            return r - l == 1;
        }

        bool is_inclusion(ll ql, ll qr) const {
            return ql <= l && r <= qr;
        }
        
        bool is_exclusion(ll ql, ll qr) const {
            return r <= ql || qr <= l;
        }
    };
    
    vec<segment> segs;
    merge_t merge_val;
    apply_t apply_lazy;
    update_t update_lazy;
    calc_lazy_t calc_lazy;
    prop_t prop_lazy;

    segment init_segment() {
        return segment(id_val, std::make_pair(id_lazy, false), -1, -1, -1);
    }

    void update_segment(segment &s) {
        s.val = merge_val(segs[s.cl].val, segs[s.cr].val);
    }

    void push(segment &s) {
        if (!s.is_lazy()) return;
        s.val = apply_lazy(s.val, s.lazy.first);
        auto prop = prop_lazy(s.lazy.first);
        s.lazy = std::make_pair(id_lazy, false);
        if (!s.is_leaf()) {
            for (ll i = s.cl; i <= s.cr; i++) {
                segment &ch = segs[i];
                ch.lazy = std::make_pair(update_lazy(ch.lazy.first, prop), true);
            }
        }
    }

public:
    LazySegmentTree(const vec<T> &v, T id_val, L id_lazy, 
                    merge_t mn, apply_t al, update_t ul, calc_lazy_t cl, prop_t pl = [](ll l) { return l; })
        : id_val(id_val), id_lazy(id_lazy), 
          segs(2 * ceil_pow2(v.size()) - 1, init_segment()),
          merge_val(mn), apply_lazy(al), update_lazy(ul), calc_lazy(cl), prop_lazy(pl)
    {
        for (ll i = 0; i < size(); i++) {
            ll idx = i + size() - 1;
            segs[idx].val = (i < v.size() ? v[i] : id_val);
            segs[idx].set_idx(idx);
            segs[idx].l = i;
            segs[idx].r = i + 1;
        }
        for (ll i = size() - 2; 0 <= i; i--) {
            segs[i].set_idx(i);
            segs[i].l = segs[segs[i].cl].l;
            segs[i].r = segs[segs[i].cr].r;
            update_segment(segs[i]);
        }
    }

    ssize_t size() const {
        return (segs.size() + 1) / 2;
    }

    void update_query(ll ql, ll qr, const L &val, segment &s) {
        push(s);
        if (s.is_exclusion(ql, qr)) return;
        if (s.is_inclusion(ql, qr)) {
            s.lazy = std::make_pair(calc_lazy(s.l, s.r, val), true);
            push(s);
        } else {
            update_query(ql, qr, val, segs[s.cl]);
            update_query(ql, qr, val, segs[s.cr]);
            update_segment(s);
        }
    }

    void update_query(ll ql, ll qr, const L &val) {
        update_query(ql, qr, val, segs[0]);
    }

    T get_query(ll ql, ll qr, segment &s) {
        push(s);
        if (s.is_exclusion(ql, qr)) return id_val;
        if (s.is_inclusion(ql, qr)) return s.val;
        return merge_val(get_query(ql, qr, segs[s.cl]),
                         get_query(ql, qr, segs[s.cr]));
    }

    T get_query(ll ql, ll qr) {
        return get_query(ql, qr, segs[0]);
    }
};

}
