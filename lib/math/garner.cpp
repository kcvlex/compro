#include "../util/template.cpp"
#include "modint.cpp"

namespace math {

ll mod_pow(ll n, ll k, ll mod) {
    ll ret = 1, cur = n;
    while (k) {
        if (k & 1) ret = ret * cur % mod;
        cur = cur * cur % mod;
        k /= 2;
    }
    return ret;
}

ll mod_inv(ll n, ll mod) {
    return mod_pow(n, mod - 2, mod);
}

template <typename ModInt>
pll get_pair(const ModInt &m) {
    return std::make_pair(m.mod(), m.value());
}

template <typename ...ModInts>
std::vector<pll> make_vecs(const ModInts&... mod_ints) {
    std::vector<pll> res = { get_pair(mod_ints)... };
    return res;
}

template <typename ...ModInts>
ll garner(ll mod, const ModInts&... mod_ints) {
    return garner(mod, std::move(make_vecs(mod_ints...)));
}

ll garner(ll mod, vec<pll> mr) {
    mr.emplace(mod, 0);
    vec<ll> coffs(mr.size(), 1);
    vec<ll> cons(mr.size(), 0);

    for (ll i = 0; i + 1 < mr.size(); i++) {
        ll v = (mr[i].second - cons[i]) * mod_inv(coffs[i], mr[i].first) % mr[i].first;
        if (v < 0) v += mr[i].first;
        for (ll j = i + 1; j < mr.size(); j++) {
            (cons[j] += coffs[j] * v) %= mr[j].first;
            (coffs[j] *= mr[i].first) %= mr[j].first;
        }
    }

    return cons.back();
}

}
