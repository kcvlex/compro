#include "../modint.hpp"
#include "../../util/ceil-pow2.hpp"

namespace math {

namespace {

template <ll Mod>
struct calc_conv_and {
    using mint = Modint<Mod>;
    const vec<ll> &s;
    const vec<ll> &t;
    const ssize_t sz;
    vec<mint> buf;

    calc_conv_and(const vec<ll> &s, const vec<ll> &t) 
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
                buf[bl + i] += buf[bm + i];
                buf[bm + i] = 0;
            }
        };

        auto rec = [&](ll rsl, ll rsr, ll rtl, ll rtr) {
            solve()(rsl, rsr, rtl, rtr, bm, br, buf);
        };

        pll sp[] = { pll(sl, sm), pll(sm, sr), };
        pll tp[] = { pll(tl, tm), pll(tm, tr), };
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
auto conv_and(const vec<ll> &s, const vec<ll> &t) {
    calc_conv_and<Mod> cca(s, t);
    cca.solve();
    auto ret = std::move(cca.buf);
    return ret;
}

}
