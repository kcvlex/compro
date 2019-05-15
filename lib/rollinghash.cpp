#include <bits/stdc++.h>
using namespace std;
template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
using ll = int64_t;

struct RollingHash {
    const V<ll> bases;
    const V<ll> mods;
    VV<ll> pows;
    VV<ll> hashs;

    template <typename T>
    RollingHash(const V<T> &s, const V<ll> bases = { 1007 }, const V<ll> mods = { (ll)1e9 + 7 }) 
        : bases(bases), mods(mods) 
    {
        assert(bases.size() == mods.size());
        for(ll i = 0; i < mods.size(); i++) {
            ll mod = mods[i];
            ll base = bases[i];
            V<ll> hash(s.size() + 1, 0);
            V<ll> _pow(s.size() + 1, 1);
            for(ll j = 0; j < s.size(); j++) {
                hash[j + 1] = (hash[j] + (s[j] - 'a')) * base % mod;
                _pow[j + 1] = _pow[j] * base % mod;
            }
            hashs.push_back(move(hash));
            pows.push_back(move(_pow));
        }
    }

    // [l, r)
    V<ll> get_hash(ll l, ll r) {
        V<ll> ret;
        for(ll i = 0; i < mods.size(); i++) {
            ll h = hashs[i][r] - hashs[i][l] * pows[i][r - l] % mods[i];
            h += mods[i];
            h %= mods[i];
            ret.push_back(h);
        }
        return move(ret);
    }
};

int main() {
    //TODO : test
    return 0;
}
