#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;

template <ll MOD>
struct mint {
    ll num;
    mint() : num(0) {}
    mint(ll num) : num((num + MOD) % MOD) {}

    mint  operator +(ll n)  { return mint(num + n); }
    mint  operator -(ll n)  { return mint(num - n); }
    mint  operator *(ll n)  { return mint(num * mint(n).num); }
    mint  operator /(ll n)  { return mint(num * pow(mint(n), MOD - 2)); }
    mint& operator +=(ll n) { (*this) + n; return *this; }
    mint& operator -=(ll n) { (*this) - n; return *this; }
    mint& operator *=(ll n) { (*this) * n; return *this; }
    mint& operator /=(ll n) { (*this) / n; return *this; }
    
    mint  operator +(mint<MOD> m)  { return (*this) + (m.num); }
    mint  operator -(mint<MOD> m)  { return (*this) - (m.num); }
    mint  operator *(mint<MOD> m)  { return (*this) * (m.num); }
    mint  operator /(mint<MOD> m)  { return (*this) / (m.num); }
    mint& operator +=(mint<MOD> m) { return (*this) += (m.num); }
    mint& operator -=(mint<MOD> m) { return (*this) -= (m.num); }
    mint& operator *=(mint<MOD> m) { return (*this) *= (m.num); }
    mint& operator /=(mint<MOD> m) { return (*this) /= (m.num); }
    
    mint pow(mint a, ll b) {
        mint tmp = a;
        mint ret(1);
        while(b) {
            ret *= (b & 1 ? a : 1);
            tmp *= tmp;
            b /= 2;
        }
        return ret;
    }

    mint pow(ll a, ll b) { return pow(mint(a), b); }
};

template <ll MOD>
istream& operator >>(istream &stream, mint<MOD> &m) {
    ll n;
    stream >> n;
    m = mint<MOD>(n);
    return stream;
}

template <ll MOD>
ostream& operator <<(ostream &stream, const mint<MOD> &m) {
    stream << m.num;
    return stream;
}
