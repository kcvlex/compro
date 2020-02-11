#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;

template <ll MOD>
struct modint {
    ll num;
    constexpr inline modint() : num(0) {}
    constexpr inline modint(ll num) : num((num + MOD) % MOD) {}

    constexpr inline modint  operator +(ll n)  { return modint(num + n); }
    constexpr inline modint  operator -(ll n)  { return modint(num - n); }
    constexpr inline modint  operator *(ll n)  { return modint(num * modint(n).num); }
    constexpr inline modint  operator /(ll n)  { return modint(num * pow(modint(n), MOD - 2)); }
    constexpr inline modint& operator +=(ll n) { (*this) + n; return *this; }
    constexpr inline modint& operator -=(ll n) { (*this) - n; return *this; }
    constexpr inline modint& operator *=(ll n) { (*this) * n; return *this; }
    constexpr inline modint& operator /=(ll n) { (*this) / n; return *this; }
    
    constexpr inline modint  operator +(modint<MOD> m)  { return (*this) + (m.num); }
    constexpr inline modint  operator -(modint<MOD> m)  { return (*this) - (m.num); }
    constexpr inline modint  operator *(modint<MOD> m)  { return (*this) * (m.num); }
    constexpr inline modint  operator /(modint<MOD> m)  { return (*this) / (m.num); }
    constexpr inline modint& operator +=(modint<MOD> m) { return (*this) += (m.num); }
    constexpr inline modint& operator -=(modint<MOD> m) { return (*this) -= (m.num); }
    constexpr inline modint& operator *=(modint<MOD> m) { return (*this) *= (m.num); }
    constexpr inline modint& operator /=(modint<MOD> m) { return (*this) /= (m.num); }
    
    constexpr inline modint pow(modint a, ll b) {
        modint tmp = a;
        modint ret(1);
        while (b) {
            ret *= (b & 1 ? a : 1);
            tmp *= tmp;
            b /= 2;
        }
        return ret;
    }

    constexpr inline modint pow(ll a, ll b) { return pow(modint(a), b); }
};

template <ll MOD>
istream& operator >>(istream &stream, modint<MOD> &m) {
    ll n;
    stream >> n;
    m = modint<MOD>(n);
    return stream;
}

template <ll MOD>
ostream& operator <<(ostream &stream, const modint<MOD> &m) {
    stream << m.num;
    return stream;
}
