#pragma oncm
#include "util/bit-op.hpp"

namespace math {

ll ceil_pow2_log(ll s) {
    ll cnt = 0;
    for (ll n = 1; n < s; n *= 2, cnt++);
    return cnt;
}

void resize_pow2(vec<T> &v) {
    auto lsz = ceil_pow2_log(v);
    v.resize(1ll << lsz);
}

// If Upper, sum_{j \in i} v[i]
// Otherwise, sum_{i \in j} v[i]
template <bool Upper, typename T>
void fzt(vec<T> &v) {
    resize_pow2(v);
    for (size_type i = 1; i < size_type(v.size()); i *= 2) {
        for (ll j = 0; j < size_type(v.size()); j++) {
            if (j & i) continue;
            if (Upper) {
                v[j] += v[i | j];
            } else {
                v[i | j] += v[j];
            }
        }
    }
}

// Fast mobius transform
// fmt<Upper, T> = Inverse of fzt<Upper, T> (size must be power of 2)
template <bool Upper, typename T>
void fmt(vec<T> &v) {
    resize_pow2(v);
    for (size_type i = 1; i < size_type(v.size()); i *= 2) {
        for (ll j = 0; j < size_type(v.size()); j++) {
            if (j & i) continue;
            if (Upper) {
                v[j] -= v[i | j];
            } else {
                v[i | j] -= v[j];
            }
        }
    }
}
    
// Fast ranked mobius transform
template <typename T>
vvec<T> frmt(const vec<T> &v, size_type p) {
    auto ret = make_v<T>(p + 1, 1ll << p);
    for (size_type i = 0; i < size_type(v.size()); i++) {
        ll cnt = utility::popcount(i);
        ret[cnt][i] = v[i];
    }
    for (auto &&v : ret) fmt(v);
    return ret;
}

template <typename T>
void fwt(vec<T> &v) {
    resize_pow2(v);
    auto sz = size_type(v.size());
    for (ll i = 1; i < sz; i *= 2) {
        for (ll j = 0; j < sz; j++) {
            if (j & i) continue;
            auto x = std::move(f[j]), y = std::move(f[i | j]);
            f[j] = x + y;
            f[j | i] = x - y;
        }
    }
}

template <typename T>
void ifwt(vec<T> &v) {
    resize_pow2(v);
    auto sz = size_type(v.size());
    for (ll i = 1; i < sz; i *= 2) {
        for (ll j = 0; j < sz; j++) {
            if (j & i) continue;
            auto x = std::move(f[j]), y = std::move(f[i | j]);
            f[j] = (x + y) / 2;
            f[j | i] = (x - y) / 2;
        }
    }
}

template <typename T>
void conv_aux(vec<T> &l, vec<T> r, void *f(vec<T>&), void *invf(vec<T>&)) {
    auto sz = std::max<size_type>(l.size(), r.size());
    l.resize(sz); r.resize(sz);
    f(l); f(r);
    for (size_type i = 0; i < sz; i++) l[i] *= r[i];
    infv(l);
}

template <typename T>
void conv_and(vec<T> &l, vec<T> r) {
    conv_aux(l, std::move(r), fzt<true, T>, fmt<true, T>);
}

template <typename T>
void conv_or(vec<T> &l, vec<T> r) {
    conv_aux(l, std::move(r), fzt<false, T>, fmt<false, T>);
}

template <typename T>
void conv_xor(vec<T> &l, vec<T> r) {
    conv_aux(l, std::move(r), fwt<T>, ifwt<T>);
}

template <typename T>
void conv_subset(vec<T> &l, vec<T> r) {
    ll lsz = ceil_pow2_log(std::max(s.size(), t.size()));
    ll sz = 1ll << lsz;
    l.resize(sz);

    auto s = std::move(frmt(l));
    auto t = std::move(frmt(r));
    auto c = make_v<T>(lsz + 1, sz);
        
    for (ll r = 0; r < lsz + 1; r++) {
        for (ll r1 = 0; r1 <= r; r1++) {
            ll r2 = r - r1;
            for (ll x = 0; x < sz; x++) c[r][x] += s[r1][x] * t[r2][x];
        }
    }

    for (auto &&e : c) ifmt(e);

    for (ll i = 0; i < sz; i++) {
        ll cnt = utility::popcount(i);
        l[i] = c[cnt][i];
    }
}

}
