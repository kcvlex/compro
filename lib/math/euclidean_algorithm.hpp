#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;
using PLL = pair<ll, ll>;
using TLL = tuple<ll, ll, ll>;
template <typename T> using V = vector<T>;

ll gcd(ll a, ll b) { return b ? gcd(a % b, b) : a; }
PLL error_pll = make_pair(-1, -1);
TLL error_tll = make_tuple(-1, -1, -1);

TLL ext_euclidean(ll a, ll b) {
    if (b == 0) {
        return make_tuple(1, 0, a);
    }
    ll xr, yr, g;
    tie(xr, yr, g) = ext_euclidean(b, a % b);
    ll x = yr;
    ll y = xr - a / b * x;
    return make_tuple(x, y, g);
}

TLL ext_euclidean(ll a, ll b, ll d) {
    ll x, y, g;
    tie(x, y, g) = ext_euclidean(a, b);
    if (d % g) {
        return error_tll;
    } else {
        ll mul = d / g;
        return make_tuple(x * mul, y * mul, g);
    }
}

PLL chinese_remainder(ll b1, ll m1, ll b2, ll m2) {
    if (b1 > b2) {
        swap(b1, b2);
        swap(m1, m2);
    }
    ll p, q, g;
    tie(p, q, g) = ext_euclidean(m1, m2);
    if ((b2 - b1) % g) {
        return error_pll;
    }
    ll m = m1 / g * m2;
    ll tmp = (b2 - b1) / g * p % (m2 / g);
    ll x = (b1 + tmp * m1 + m) % m;
    return make_pair(x, m);
}

PLL chinese_remainder(const V<ll> &bv, const V<ll> &mv) {
    assert(bv.size() == mv.size());
    ll b = 0, m = 1;
    for (ll i = 0; i < bv.size(); i++) {
        tie(b, m) = chinese_remainder(b, m, bv[i], mv[i]);
        if (b == -1) {
            return error_pll;
        }
    }
    return make_pair(b, m);
}

int main() {
    V<ll> bv = { 2, 3 };
    V<ll> mv = { 3, 5 };
    ll x, m;
    tie(x, m) = chinese_remainder(bv, mv);
    cout << x << ' ' << m << endl;
    return 0;
}
