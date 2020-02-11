#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;

template <ll MOD>
class Combination {
    ll N;
    V<ll> factv, ifactv;

public:
    Combination<MOD>(ll N) : N(N), factv(N + 1, 1), ifactv(N + 1) {
        for (ll i = 1; i <= N; i++) factv[i] = factv[i - 1] * i % MOD;
        ifactv.back() = inv(factv.back());
        for (ll i = N - 1; 0 <= i; i--) ifactv[i] = (i + 1) * ifactv[i + 1] % MOD;
    }

    ll fact(ll n) { return factv[n]; }

    ll ifact(ll n) { return ifactv[n]; }

    ll pow(ll a, ll b) { return b ? (b & 1 ? a : 1) * pow(a * a % MOD, b / 2) % MOD : 1; }

    ll inv(ll a) { return pow(a, MOD - 2); }

    ll comb(ll n, ll k) { return factv[n] * ifactv[n - k] % MOD * ifactv[k] % MOD; }
};


int main() {
    //TODO: test
    return 0;
}
