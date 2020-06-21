#pragma once
#include "alias.hpp"

template <typename T, std::size_t Head, std::size_t... Tail> 
struct multi_dim_array { using type = std::array<typename multi_dim_array<T, Tail...>::type, Head>; };

template <typename T, std::size_t Head> 
struct multi_dim_array<T, Head> { using type = std::array<T, Head>; };

template <typename T, std::size_t... Args> using mdarray = typename multi_dim_array<T, Args...>::type;

#ifdef CPP17
template <typename T, typename F, typename... Args> 
void fill_seq(T &t, F f, Args... args) { 
    if constexpr (std::is_invocable<F, Args...>::value) { 
        t = f(args...); 
    } else { 
        for (size_type i = 0; i < t.size(); i++) fill_seq(t[i], f, args..., i); 
    } 
}
#endif

template <typename T> vec<T> make_v(size_type sz) { return vec<T>(sz); }

template <typename T, typename... Tail> 
auto make_v(size_type hs, Tail&&... ts) { 
    auto v = std::move(make_v<T>(std::forward<Tail>(ts)...)); 
    return vec<decltype(v)>(hs, v); 
}
