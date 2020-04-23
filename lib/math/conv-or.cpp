#include "../modint.cpp"
#include "../../util/ceil-pow2.cpp"

namespace math {

namespace {

template <ll Mod>
struct calc_conv_or {
    using mint = Modint<Mod>;
    const vec<ll> &s;
    const vec<ll> &t;
    const ssize_t sz;
    vec<mint> buf;

    calc_conv_or(const vec<ll> &s, const vec<ll> &t) 
        : s(s), t(t), sz(ceil_pow2(std::max(s.size(), t.size()))), buf(sz)
    {
    }

    void solve(ll sl, ll sr, ll tl, ll tr, ll bl, ll br) {
        ll len = br - bl;
        if (len == 1) {
            ll sv = get_sv(sl), tv = get_tv(tl);
            buf[bl] = sv * tv;
            return;
        }

        ll sm = (sl + sr) / 2, tm = (tl + tr) / 2, bm = (bl + br) / 2;

        auto move_v = [&] {
            for (ll i = 0; i < len / 2; i++) {
                buf[bm + i] += buf[bl + i];
                buf[bl + i] = 0;
            }
        };

        auto rec = [&](ll rsl, ll rsr, ll rtl, ll rtr) {
            solve(rsl, rsr, rtl, rtr, bl, bm);
        };

        pll sp[] = { pll(sm, sr), pll(sl, sm), };
        pll tp[] = { pll(tm, tr), pll(tl, tm), };
        for (ll i = 0; i < 4; i++) {
            ll a, b, c, d;
            std::tie(a, b) = sp[i & 1];
            std::tie(c, d) = tp[(i >> 1) & 1];
            rec(a, b, c, d);
            if (i + 1 < 4) move_v();
        }
    }

    void solve() {
        solve(0, sz, 0, sz, 0, sz);
    }
};


}

template <ll Mod>
auto conv_or(const vec<ll> &s, const vec<ll> &t) {
    calc_conv_or<Mod> cco(s, t);
    cco.solve();
    auto ret = std::move(cco.buf);
    return ret;
}

}
