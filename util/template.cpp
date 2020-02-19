// #define DEBUGGING
#include <bits/stdc++.h>
#define endl '\n'
#define ALL(V) begin(V), end(V)
#define ALLR(V) rbegin(V), rend(V)
using ll = std::int64_t;
using ull = std::uint64_t;
using PLL = std::pair<ll, ll>;
using TLL = std::tuple<ll, ll, ll>;
template <typename T> using V = std::vector<T>;
template <typename T> using VV = V<V<T>>;
template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return std::min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return std::max(t, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }
template <typename T> const T& clamp(const T &t, const T &low, const T &high) { return std::max(low, std::min(high, t)); }
template <typename T> void chclamp(T &t, const T &low, const T &high) { return t = clamp(t, low, high); }
namespace init__ { struct InitIO { InitIO() { std::cin.tie(nullptr); std::ios_base::sync_with_stdio(false); std::cout << std::fixed << std::setprecision(30); } } init_io; }
#define mv_rec make_v(init, tail...)
template <typename T> T make_v(T init) { return init; }
template <typename T, typename... Tail> auto make_v(T init, size_t s, Tail... tail) { return V<decltype(mv_rec)>(s, mv_rec); }
#undef mv_rec
using namespace std;

#ifdef DEBUGGING
#include "../../debug/debug.cpp"
#else
#define DEBUG(...) 0
#define DEBUG_SEPARATOR_LINE 0
#endif
