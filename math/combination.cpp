#include "../util/template.cpp"

template <ll Mod>
class Combination {
    ssize_t maxv;
    vec<ll> factv, ifactv;

public:
    Combination(ssize_t maxv) : maxv(maxv), factv(maxv + 1, 1), ifactv(maxv + 1) {
        for (ll i = 1; i <= maxv; i++) factv[i] = factv[i - 1] * i % Mod;
        ifactv.back() = inv(factv.back());
        for (ll i = maxv - 1; 0 <= i; i--) ifactv[i] = (i + 1) * ifactv[i + 1] % Mod;
    }

    ll fact(ll n) { return factv[n]; }

    ll ifact(ll n) { return ifactv[n]; }

    ll pow(ll a, ll b) { return b ? (b & 1 ? a : 1) * pow(a * a % Mod, b / 2) % Mod : 1; }

    ll inv(ll a) { return pow(a, Mod - 2); }

    ll comb(ll n, ll k) { return factv[n] * ifactv[n - k] % Mod * ifactv[k] % Mod; }
};
