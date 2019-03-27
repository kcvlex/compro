#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;

class Combination {
private:
    template <typename T> using V = vector<ll>;
    ll N;
    ll MOD;
    V<ll> factv, rfactv;

public:
    /*
     * MOD must be a prime number.
     */
    Combination(ll N, ll MOD)
        : N(N), 
          MOD(MOD),
          factv(N + 1, 1),
          rfactv(N + 1)
    {
        for(ll i = 1; i <= N; i++) {
            factv[i] = factv[i - 1] * i % MOD;
        }
        for(ll i = 0; i <= N; i++) {
            rfactv[i] = pow(factv[i], MOD - 2);
        }
    }

    ll fact(ll n) {
        return factv[n];
    }

    ll rfact(ll n) {
        return rfactv[n];
    }

    ll pow(ll a, ll b) {
        return b ? (b & 1 ? a : 1) * pow(a * a % MOD, b / 2) % MOD : 1;
    }

    ll comb(ll n, ll k) {
        return factv[n] * rfactv[n - k] % MOD * rfactv[k] % MOD;
    }
};

int main() {
    //TODO: test
    return 0;
}
