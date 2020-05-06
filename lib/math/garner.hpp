#pragma once
#include "modint.hpp"
#include "../util/generics.hpp"

namespace math {

namespace {

template <ll... Mods>
struct modint_tuple;

template <ll Mod, ll Dummy, ll... Mods>
struct modint_tuple<Mod, Dummy, Mods...> {
    using type = typename utility::tuple_concat<std::tuple<Modint<Mod>>,
                            typename modint_tuple<Dummy, Mods...>::type>::type;
};

template <ll Mod>
struct modint_tuple<Mod> {
    using type = std::tuple<Modint<Mod>>;
};

template <std::size_t Bound, std::size_t I>
struct dummy_type {
    using type = 
        typename std::conditional<(I < Bound), std::true_type, std::false_type>::type;
};
                    

template <ll... Mods>
struct tuple2array {
    constexpr static std::size_t len = sizeof...(Mods);
    template <std::size_t I> using dummy = typename dummy_type<len, I>::type;

    using src_t = typename modint_tuple<Mods...>::type;
    using dst_t = std::array<ll, len>;

    const src_t &src;
    dst_t dst;

    tuple2array(const src_t &src) : src(src) { }

    template <std::size_t Index>
    void convert(std::true_type) {
        dst[Index] = std::get<Index>(src).value();
        convert<Index + 1>();
    }

    template <std::size_t Index>
    void convert(std::false_type) { }

    template <std::size_t Index = 0>
    void convert() {
        convert<Index>(dummy<Index>());
    }

    dst_t get() {
        convert();
        return dst;
    }
};

template <ll... Mods> 
struct array2tuple {
    constexpr static std::size_t len = sizeof...(Mods);
    template <std::size_t I> using dummy = typename dummy_type<len, I>::type;
    
    using src_t = std::array<ll, len>;
    using dst_t = typename modint_tuple<Mods...>::type;

    const src_t &src;
    dst_t dst;

    array2tuple(const src_t &src) : src(src) { }
    
    template <std::size_t Index>
    void convert(std::true_type) {
        std::get<Index>(dst) = src[Index];
        convert<Index + 1>();
    }

    template <std::size_t Index>
    void convert(std::false_type) { }

    template <std::size_t Index = 0>
    void convert() {
        convert<Index>(dummy<Index>());
    }

    dst_t get() {
        convert();
        return dst;
    }
};

template <ll... Mods>
class Garner {
    constexpr static std::size_t len = sizeof...(Mods);
    constexpr static std::array<ll, len> mods = {{ Mods... }};
    using tuple_t = typename modint_tuple<Mods...>::type;
    using array_t = std::array<ll, len>;
    template <std::size_t I> using dummy = typename dummy_type<len, I>::type;

    const array_t &mr;
    tuple_t coffs, constants;
    const ll mod;
    ll coff, con;

    template <std::size_t Index>
    void update(ll v) {
        auto coff_arr = tuple2array<Mods...>(coffs).get();
        auto const_arr = tuple2array<Mods...>(constants).get();
        for (ll i = Index + 1; i < len; i++) {
            const_arr[i] += coff_arr[i] * v;
            coff_arr[i] *= mods[Index];
        }
        (con += coff * v) %= mod;
        (coff *= mods[Index]) %= mod;
        coffs = array2tuple<Mods...>(coff_arr).get();
        constants = array2tuple<Mods...>(const_arr).get();
    }

    template <std::size_t Index>
    ll solve(std::true_type) {
        auto v = (std::get<Index>(constants) - mr[Index]) * -1 * std::get<Index>(coffs).inv();
        update<Index>(v.value());
        return solve<Index + 1>();
    }

    template <std::size_t Index>
    ll solve(std::false_type) {
        return con;
    }

public:
    Garner(const array_t &mr, ll mod) : mr(mr), mod(mod), coff(1), con(0) {
        array_t arr;
        std::fill(ALL(arr), 1);
        coffs = array2tuple<Mods...>(arr).get();
        std::fill(ALL(arr), 0);
        constants = array2tuple<Mods...>(arr).get();
    }

    template <std::size_t Index = 0>
    ll solve() {
        return solve<Index>(dummy<Index>());
    }
};

}

template <ll... Mods>
ll garner(std::integer_sequence<ll, Mods...>, const std::array<ll, sizeof...(Mods)> &mr, ll mod) {
    return Garner<Mods...>(mr, mod).solve();
}

}
