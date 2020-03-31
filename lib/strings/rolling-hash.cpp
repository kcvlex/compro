#pragma once
#include "../util/template.cpp"
#include "../util/generics.cpp"

namespace strings {

template <ll Base, ll ...Mods>
struct RollingHash {
    constexpr static std::size_t mods_sz = sizeof...(Mods);
    
    using data_t = typename std::array<ll, mods_sz>; 

    constexpr static data_t mods = {{ Mods... }};
    data_t dat;
    std::size_t sz;

    RollingHash() : dat(), sz(0) { }
    RollingHash(const data_t &dat, std::size_t sz) : dat(dat), sz(sz) { }

    RollingHash append(ll e) {
        data_t nxt;
        for (ll i = 0; i < mods_sz; i++) {
            ll mod = mods[i];
            ll v = dat[i] * Base % mod + e % mod;
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

template <ll Base, ll ...Mods>
struct BasePows {
    constexpr static std::size_t mods_sz = sizeof...(Mods);
    constexpr static std::array<ll, mods_sz> mods = {{ Mods... }};

    std::array<ll, sizeof...(Mods)> pows;

    BasePows() {
        std::fill(ALL(pows), 1);
    }

    BasePows next() {
        BasePows<Base, Mods...> ret;
        for (ll i = 0; i < mods_sz; i++) ret[i] = pows[i] * Base % mods[i];
        return ret;
    }

    ll operator [](std::size_t idx) const {
        return pows[idx];
    }

    ll& operator [](std::size_t idx) {
        return pows[idx];
    }
};

template <ll ...Mods>
struct RollingHashBuilder {
    constexpr static ll base = 1007;
    
    using hash_t = RollingHash<base, Mods...>;
    using data_t = typename hash_t::data_t;
    using pows_t = BasePows<base, Mods...>;

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

template <const ll *Mods>
constexpr auto gen_builder() {
    return typename utility::pass_array<ll, RollingHashBuilder, Mods>::type();
}

}
