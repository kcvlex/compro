#pragma once
#include "../util/template.cpp"

namespace str {

namespace {

template <typename T, std::size_t Size, typename ...Args>
struct sized_tuple {
    using type = typename sized_tuple<T, Size - 1, T, Args...>::type;
};

template <typename T, typename ...Args>
struct sized_tuple<T, 0, Args...> {
    using type = std::tuple<Args...>;
};

template <std::size_t Size, std::size_t Count, typename BidirIterator, typename ...Args>
constexpr auto arr2tuple(BidirIterator ite, Args&&... args) {
    if constexpr (Size == Count) {
        return std::make_tuple(args...);
    } else {
        auto cpy = *ite;
        return arr2tuple<Size, Count + 1>(std::next(ite), cpy, std::forward<Args>(args)...);
    }
}

template <typename BidirIterator, typename Tuple, std::size_t Index = 0> 
constexpr void tuple2arr(BidirIterator ite, const Tuple &tpl) {
    if constexpr (Index == std::tuple_size<Tuple>::value) {
        return;
    } else {
        *ite = std::get<Index>(tpl);
        tuple2arr<BidirIterator, Tuple, Index + 1>(std::next(ite), tpl);
    }
}

template <typename T, std::size_t Size>
struct appender {
    T t;
    ll base;
    const std::array<ll, Size> &mods;

    appender(T t, ll base, const std::array<ll, Size> &mods) : t(t), base(base), mods(mods) { }
    appender(ll base, const std::array<ll, Size> &mods) : appender(T(), base, mods) { }

    template <typename ...Args>
    auto append(Args&&... args) const {
        static_assert(sizeof...(args) == Size);
        std::array<ll, Size> arr = {{ args... }};
        for (ll i = 0; i < Size; i++) {
            arr[i] *= base;
            arr[i] += t;
            arr[i] %= mods[i];
        }
        return arr2tuple<Size, 0>(arr.rbegin());
    }

    template <typename ...Args>
    auto operator ()(Args&&... args) const {
        return append(std::forward<Args>(args)...);
    }
};

template <typename T, std::size_t Size>
typename sized_tuple<T, Size>::type init_tuple(T init_v) {
    std::array<T, Size> arr;
    for (ll i = 0; i < Size; i++) arr[i] = init_v;
    return arr2tuple<Size, 0>(arr.rbegin());
}

}  // anonymous

template <ll ...Mods>
struct RollingHash {
    using data_t = typename sized_tuple<ll, sizeof...(Mods)>::type;

    data_t dat;
    std::size_t sz;

    RollingHash() : dat(), sz(0) { }
    RollingHash(data_t &&dat, std::size_t sz) : dat(std::move(dat)), sz(sz) { }
    RollingHash(const data_t &dat, std::size_t sz) : dat(dat), sz(sz) { }

    bool operator ==(const RollingHash &oth) const {
        return sz == oth.sz && dat == oth.dat;
    }
};

template <ll ...Mods>
struct RollingHashBuilder {
    using hash_t = RollingHash<Mods...>;
    using data_t = typename hash_t::data_t;

    constexpr static ll base = 1007;
    constexpr static std::size_t mods_sz = sizeof...(Mods);
    constexpr static std::array<ll, mods_sz> mods = {{ Mods... }};
    vec<data_t> pows;
    
    void append_pows(std::size_t sz) {
        static const appender<ll, mods_sz> app(base, mods);
        if (pows.empty()) pows.push_back(init_tuple<ll, mods_sz>(1));
        for (ll i = pows.size(); i <= sz; i++) {
            data_t arg = pows.back();
            data_t tmp = std::apply(app, arg);
            pows.push_back(tmp);
        }
    }

    template <typename BidirIterator>
    vec<hash_t> build(BidirIterator first, BidirIterator last) {
        auto sz = std::distance(first, last);
        append_pows(sz + 1);
        vec<hash_t> ret(sz + 1);
        auto ite = first;
        for (ll i = 0; i < sz; i++) {
            appender<ll, mods_sz> app(*ite, base, mods);
            auto dat = std::apply(app, ret[i].dat);
            ret[i + 1] = hash_t(dat, i + 1);
            std::advance(ite, 1);
        }
        return ret;
    }

    // [l, r)
    hash_t substr(const vec<hash_t> &hash, ll l, ll r) {
        append_pows(r - l + 1);
        std::array<ll, mods_sz> hl, hr, ps, ans;
        tuple2arr(hl.begin(), hash[l].dat);
        tuple2arr(hr.begin(), hash[r].dat);
        tuple2arr(ps.begin(), pows[r - l]);
        for (ll i = 0; i < mods_sz; i++) {
            ll mod = mods[i];
            ans[i] = hr[i] - hl[i] * ps[i] % mod + mod;
            if (mod <= ans[i]) ans[i] -= mod;
        }
        return hash_t(arr2tuple<mods_sz, 0>(ans.rbegin()), r - l);
    }

    hash_t concat(const hash_t &d1, const hash_t &d2) {
        append_pows(d2.sz + 1);
        std::array<ll, mods_sz> h1, h2, ps, ans;
        tuple2arr(h1.begin(), d1.dat);
        tuple2arr(h2.begin(), d2.dat);
        tuple2arr(ps.begin(), pows[d2.sz]);
        for (ll i = 0; i < mods_sz; i++) {
            ll mod = mods[i];
            ans[i] = h1[i] * ps[i] % mod + h2[i];
            if (mod <= ans[i]) ans[i] -= mod;
        }
        return hash_t(arr2tuple<mods_sz, 0>(ans.rbegin()), d1.sz + d2.sz);
    }
};

}
