#pragma once
#include "../util/template.hpp"
#include "../util/generics.hpp"

namespace strings {

template <ll... Mods>
struct base_generator {
    constexpr static std::size_t mods_sz = sizeof...(Mods);
    constexpr static std::array<ll, mods_sz> mods = {{ Mods... }};
    template <std::size_t I> using dummy_type = typename std::conditional<
        (I < mods_sz), std::true_type, std::false_type>::type;

    constexpr base_generator() { }

    constexpr ll xorshift32(ll seed) {
        seed = seed ^ (seed << 13);
        seed = seed ^ (seed >> 17);
        return seed;
    }

    constexpr ll generate_seed() {
        ll ret = 1;
        for (int i = 0; i < 8; i++) {
            char c = __TIME__[i];
            if ('0' <= c && c <= '9') ret = ret * 10 + (c - '0');
        }
        return ret;
    }

    template <std::size_t Index, typename... Results>
    constexpr auto generate(std::true_type, ll seed, Results... results) -> std::array<ll, mods_sz> {
        const ll nseed = xorshift32(seed);
        const ll mod = mods[Index];
        const ll base = nseed % mod;
        return generate<Index + 1>(dummy_type<Index + 1>(), nseed, results..., base);
    }

    template <std::size_t Index, typename... Results>
    constexpr auto generate(std::false_type, ll, Results... results) -> std::array<ll, mods_sz> {
        return {{ results... }};
    }

    constexpr auto get() {
        return generate<0>(dummy_type<0>(), generate_seed());
    }
};

template <ll... Mods>
struct RollingHash {
    constexpr static std::size_t mods_sz = sizeof...(Mods);
    
    using data_t = typename std::array<ll, mods_sz>; 

    constexpr static data_t mods = {{ Mods... }};
    constexpr static data_t bases = base_generator<Mods...>().get();

    data_t dat;
    std::size_t sz;
    
    static std::array<ll, mods_sz> generate_bases() {
        std::array<ll, mods_sz> bases;
        std::random_device rd;
        for (ll i = 0; i < mods_sz; i++) {
            ll b = rd() + 1;
            b %= mods[i];
            bases[i] = b;
        }
        return bases;
    }

    RollingHash(const data_t &dat, std::size_t sz) : dat(dat), sz(sz) { }
    RollingHash() : RollingHash(data_t(), 0) { }

    RollingHash append(ll e) {
        data_t nxt;
        for (ll i = 0; i < mods_sz; i++) {
            ll mod = mods[i];
            ll v = dat[i] * bases[i] % mod + e % mod;
            if (mod <= v) v -= mod;
            nxt[i] = v;
        }

        return RollingHash(nxt, sz + 1);
    }

    bool operator ==(const RollingHash &oth) const {
        return sz == oth.sz && dat == oth.dat;
    }

    bool operator !=(const RollingHash &oth) const {
        return !(*this == oth);
    }

    ll operator [](std::size_t idx) const {
        return dat[idx];
    }

    ll& operator [](std::size_t idx) {
        return dat[idx];
    }
};

template <ll... Mods>
struct BasePows {
    constexpr static std::size_t mods_sz = sizeof...(Mods);
    constexpr static auto &mods = RollingHash<Mods...>::mods;
    constexpr static auto &bases = RollingHash<Mods...>::bases;

    std::array<ll, sizeof...(Mods)> pows;

    BasePows() {
        std::fill(ALL(pows), 1);
    }

    BasePows next() {
        BasePows<Mods...> ret;
        for (ll i = 0; i < mods_sz; i++) ret[i] = pows[i] * bases[i] % mods[i];
        return ret;
    }

    ll operator [](std::size_t idx) const {
        return pows[idx];
    }

    ll& operator [](std::size_t idx) {
        return pows[idx];
    }
};

template <ll... Mods>
struct RollingHashBuilder {
    using hash_t = RollingHash<Mods...>;
    using data_t = typename hash_t::data_t;
    using pows_t = BasePows<Mods...>;

    constexpr static std::size_t mods_sz = sizeof...(Mods);
    constexpr static std::array<ll, mods_sz> mods = {{ Mods... }};
    vec<pows_t> pows;

    RollingHashBuilder() {
        pows.push_back(pows_t());
    }
    
    void repl_pows(std::size_t sz) {
        for (ll i = pows.size(); i <= sz; i++) {
            auto nxt = std::move(pows.back().next());
            pows.push_back(std::move(nxt));
        }
    }

    template <typename BidirIterator>
    vec<hash_t> build(BidirIterator first, BidirIterator last) {
        auto sz = std::distance(first, last);
        vec<hash_t> ret(sz + 1);
        auto ite = first;
        ll idx = 0;
        for (auto ite = first; ite != last; ite = std::next(ite), idx++) {
            auto e = *ite;
            ret[idx + 1] = ret[idx].append(static_cast<ll>(e));
        }
        return ret;
    }

    // [l, r)
    hash_t substr(const vec<hash_t> &hash, ll l, ll r) {
        repl_pows(r - l + 1);
        const auto &hl = hash[l];
        const auto &hr = hash[r];
        const auto &ps = pows[r - l];
        data_t ans;
        for (ll i = 0; i < mods_sz; i++) {
            ll mod = mods[i];
            ans[i] = hr[i] - hl[i] * ps[i] % mod + mod;
            if (mod <= ans[i]) ans[i] -= mod;
        }
        return hash_t(ans, r - l);
    }

    hash_t concat(const hash_t &d1, const hash_t &d2) {
        repl_pows(d2.sz + 1);
        const auto &h1 = d1;
        const auto &h2 = d2;
        const auto &ps = pows[d2.sz];
        data_t ans;
        for (ll i = 0; i < mods_sz; i++) {
            ll mod = mods[i];
            ans[i] = h1[i] * ps[i] % mod + h2[i];
            if (mod <= ans[i]) ans[i] -= mod;
        }
        return hash_t(ans, d1.sz + d2.sz);
    }
};

template <ll... Mods>
auto gen_rhbuilder(std::integer_sequence<ll, Mods...>) {
    return RollingHashBuilder<Mods...>();
}

}
