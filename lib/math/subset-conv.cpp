#include "modint.cpp"

namespace math {

ll ceil_pow2_log(ll s) {
    ll cnt = 0;
    for (ll n = 1; n < s; n *= 2, cnt++);
    return cnt;
}

template <ll Mod>
struct subset_conv_solver {
    using mint = Modint<Mod>;

    void frmt(vec<mint> &v) {
        auto sz = v.size();
        for (ll i = 1; i < sz; i *= 2) for (ll j = 0; j < sz; j++) if (i & j) v[j] += v[j ^ i];
    }

    void ifrmt(vec<mint> &v) {
        auto sz = v.size();
        for (ll i = 1; i < sz; i *= 2) for (ll j = 0; j < sz; j++) if (i & j) v[j] -= v[i ^ j];
    }

    vec<mint> solve(const vec<ll> &s, const vec<ll> &t) {
        ll lsz = ceil_pow2_log(std::max(s.size(), t.size()));
        ll sz = 1ll << lsz;
        auto sr = make_v<mint>(0, lsz + 1, 1ll << lsz);
        auto tr = sr, cr = sr;

        for (ll i = 0; i < std::max(s.size(), t.size()); i++) {
            ll cnt = __builtin_popcount(i);
            if (i < s.size()) sr[cnt][i] = s[i];
            if (i < t.size()) tr[cnt][i] = t[i];
        }

        for (auto &&e : sr) frmt(e);
        for (auto &&e : tr) frmt(e);

        for (ll r = 0; r < lsz + 1; r++) {
            for (ll r1 = 0; r1 <= r; r1++) {
                ll r2 = r - r1;
                for (ll x = 0; x < sz; x++) cr[r][x] += sr[r1][x] * tr[r2][x];
            }
        }

        for (auto &&p : cr) ifrmt(p);
        vec<mint> ret(sz);
        for (ll i = 0; i < sz; i++) {
            ll cnt = __builtin_popcount(i);
            ret[i] = cr[cnt][i];
        }
        return ret;
    }
};

template <ll Mod>
auto subset_conv(const vec<ll> &s, const vec<ll> &t) {
    subset_conv_solver<Mod> solver;
    return solver.solve(s, t);
}

}
