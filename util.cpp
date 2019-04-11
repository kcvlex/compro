#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define ALL(V) (V).begin(), (V).end()
#define ALLR(V) (V).rbegin(), (V).rend()

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
template <typename T, typename U> using P = pair<T, U>;
using ll = int64_t;
using PLL = P<ll, ll>;

template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename Head, typename... Tail> const Head& var_min(const Head &head, const Tail&... tail) { return min(head, var_min(tail...)); }
template <typename Head, typename... Tail> const Head& var_max(const Head &head, const Tail&... tail) { return max(head, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }

namespace __init {

struct InitIO {
    InitIO() {
        cin.tie(0);
        ios_base::sync_with_stdio(false);
        cout << fixed << setprecision(30);
    }
} init_io;

}

istream& operator >>(istream &is, __int128_t &a) {
    int64_t i;
    is >> i;
    a = i;
    return is;
}

ostream& operator <<(ostream &os, __int128_t a) {
    auto i = (int64_t)a;
    os << i;
    return os;
}


#define DEBUGGING
#ifdef DEBUGGING
#include "../debug.cpp"
#else
#define DEBUG(...) 0
#endif
