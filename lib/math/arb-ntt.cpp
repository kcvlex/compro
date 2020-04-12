#include "ntt.cpp"
#include "garner.cpp"

namespace math {

namespace {

template <typename T>
struct pointer_to_const {
    using type = typename std::add_pointer<
        typename std::add_const<T>::type>::type;
};

template <std::size_t MaxSizeLog, ll... Mods> class multi_ntt_type;

template <std::size_t MaxSizeLog, ll Mod, ll... Mods>
class multi_ntt_type<MaxSizeLog, Mod, Mods...> {
    using ntt_head = NTT<Mod, MaxSizeLog>;
    using result_head = typename pointer_to_const<typename ntt_head::poly>::type;
    using tails = multi_ntt_type<MaxSizeLog, Mods...>;
    using ntt_tail = typename tails::ntt_type;
    using result_tail = typename tails::result_type;

public:
    using ntt_type = typename utility::tuple_concat<std::tuple<ntt_head>, ntt_tail>::type;
    using result_type = typename utility::tuple_concat<std::tuple<result_head>, result_tail>::type;
};

template <std::size_t MaxSizeLog, ll Mod>
class multi_ntt_type<MaxSizeLog, Mod> {
    using type = NTT<Mod, MaxSizeLog>;
    using poly = typename pointer_to_const<typename type::poly>::type;

public:
    using ntt_type = std::tuple<type>;
    using result_type = std::tuple<poly>;
};

}

template <std::size_t MaxSizeLog, ll... Mods>
class ArbitraryNTT {
    using multi_ntt = multi_ntt_type<MaxSizeLog, Mods...>;
    using ntts_type = typename multi_ntt::ntt_type;
    using results_type = typename multi_ntt::result_type;
    using data_type = std::array<ll, (1ll << (MaxSizeLog + 1))>;
    using mods_seq = std::integer_sequence<ll, Mods...>;

    ntts_type ntts;
    std::array<ll, sizeof...(Mods)> dat;
    data_type buf;

    template <std::size_t Index, typename Container1, typename Container2, typename... Results>
    auto for_each_conv_rec(const Container1 &a, const Container2 &b, Results&&... results) {
        if constexpr (Index < sizeof...(Mods)) {
            auto res = &(std::get<Index>(ntts).convolution(a, b));
            return for_each_conv_rec<Index + 1>(a, b, results..., res);
        } else {
            return std::make_tuple(std::forward<Results>(results)...);
        }
    }

    template <typename Container1, typename Container2>
    results_type for_each_conv(const Container1 &a, const Container2 &b) {
        return for_each_conv_rec<0>(a, b);
    }

    template <std::size_t Index = 0>
    void collect(const ll idx, results_type res) {
        if constexpr (Index < sizeof...(Mods)) {
            dat[Index] = (*std::get<Index>(res))[idx].value();
            collect<Index + 1>(idx, res);
        } else {
        }
    }

public:
    ArbitraryNTT() : ntts() { }

    template <typename Container1, typename Container2>
    const data_type& convolution(const Container1 &a, const Container2 &b, ll mod) {
        auto conv_size = ceil_pow2(a.size() + b.size() - 1);
        decltype(auto) res = for_each_conv(a, b);
        for (ll i = 0; i < conv_size; i++) {
            collect(i, res);
            buf[i] = garner(mods_seq(), dat, mod);
        }
        return buf;
    }
};

template <std::size_t MaxSizeLog, ll... Mods>
auto gen_arb_ntt(std::integer_sequence<ll, Mods...>) {
    return ArbitraryNTT<MaxSizeLog, Mods...>();
}

}
