#pragma once
#include <bits/stdc++.h>
#define ALL(v) std::begin(v), std::end(v)
#define ALLR(v) std::rbegin(v), std::rend(v)
using ll = std::int64_t;
using ull = std::uint64_t;
using pll = std::pair<ll, ll>;
using tll = std::tuple<ll, ll, ll>;
namespace init__ { struct InitIO { InitIO() { std::cin.tie(nullptr); std::ios_base::sync_with_stdio(false); std::cout << std::fixed << std::setprecision(30); } } init_io; }
template <typename T> using vec = std::vector<T>;
template <typename T> using vvec = vec<vec<T>>;
template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return std::min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return std::max(t, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }
template <typename T> const T& clamp(const T &t, const T &low, const T &high) { return std::max(low, std::min(high, t)); }
template <typename T> void chclamp(T &t, const T &low, const T &high) { return t = clamp(t, low, high); }
template <typename T> T make_v(T init) { return init; }
template <typename T, typename... Tail> auto make_v(T init, std::size_t s, Tail... tail) { auto v = std::move(make_v(init, tail...)); return vec<decltype(v)>(s, v); }
template <typename T, std::size_t Head, std::size_t ...Tail> struct multi_dem_array { using type = std::array<typename multi_dem_array<T, Tail...>::type, Head>; };
template <typename T, std::size_t Head> struct multi_dem_array<T, Head> { using type = std::array<T, Head>; };
template <typename T, std::size_t ...Args> using mdarray = typename multi_dem_array<T, Args...>::type;

#define DEBUGGING
#include "/home/taroy/kyopuro/lib/util/debug.cpp"
