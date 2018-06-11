#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
class Combination{
    private:
        ll N;
        ll MOD;
        ll *fact;

    public:
        /*
         * N:   nCkの最大のN
         * MOD: 素数でなければならない
         */
        Combination(ll N, ll MOD){
            this->N = N;
            this->MOD = MOD;
            fact = new ll[N + 1];
            fact[0] = 1;
            for(ll i = 1; i <= N; i++){
                fact[i] = fact[i - 1] * i % MOD;
            }
        }

        ll pow(ll a, ll b){
            return b ? (b & 1 ? a : 1) * pow(a * a % MOD, b / 2) % MOD : 1;
        }

        ll comb(ll n, ll k){
            return fact[n] * pow(fact[n - k] * fact[k] % MOD, MOD - 2) % MOD;
        }
};
