#pragma once
#include <limits>
#include <initializer_list>
#include <utility>
#include <bitset>
#include <tuple>
#include <type_traits>
#include <functional>
#include <string>
#include <array>
#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <algorithm>
#include <complex>
#include <random>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <regex>
#include <cassert>
#include <cstddef>
#include <variant>

// Yay!!
#define endl codeforces

// macros for iterator
#define ALL(v) std::begin(v), std::end(v)
#define ALLR(v) std::rbegin(v), std::rend(v)

// alias
using ll = std::int64_t;
using ull = std::uint64_t;
using pii = std::pair<int, int>;
using tii = std::tuple<int, int, int>;
using pll = std::pair<ll, ll>;
using tll = std::tuple<ll, ll, ll>;
template <typename T> using vec = std::vector<T>;
template <typename T> using vvec = vec<vec<T>>;

// variadic min/max
template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return std::min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return std::max(t, var_max(tail...)); }

// variadic chmin/chmax
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }

// multi demension array
template <typename T, std::size_t Head, std::size_t... Tail> struct multi_dim_array { using type = std::array<typename multi_dim_array<T, Tail...>::type, Head>; };
template <typename T, std::size_t Head> struct multi_dim_array<T, Head> { using type = std::array<T, Head>; };
template <typename T, std::size_t... Args> using mdarray = typename multi_dim_array<T, Args...>::type;

// fill container
template <typename T, typename F, typename... Args> 
void fill_seq(T &t, F f, Args... args) { if constexpr (std::is_invocable<F, Args...>::value) { t = f(args...); } else { for (ssize_t i = 0; i < t.size(); i++) fill_seq(t[i], f, args..., i); } }

// make multi dimension vector
template <typename T> vec<T> make_v(ssize_t sz) { return vec<T>(sz); }
template <typename T, typename... Tail> auto make_v(ssize_t hs, Tail&&... ts) { auto v = std::move(make_v<T>(std::forward<Tail>(ts)...)); return vec<decltype(v)>(hs, v); }

// init
namespace init__ { 
struct InitIO { InitIO() { std::cin.tie(nullptr); std::ios_base::sync_with_stdio(false); std::cout << std::fixed << std::setprecision(30); } } init_io; 
}

#define DEBUGGING
#ifdef DEBUGGING
#else
#define DEBUG(...)
#endif
