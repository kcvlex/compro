#include "modint.cpp"
#include "garner.cpp"
#include "ntt.cpp"

// FIXME : test
template <ll MaxSizeLog>
struct Builder {
    template <ll Mod>
    math::NTT<Mod, MaxSizeLog> build() {
        return math::NTT<Mod, MaxSizeLog>();
    }
};

template <ll MaxSizeLog, ll ...Mods>
class ArbConv {
    constexpr static Builder<MaxSizeLog> ntt_builder;
    constexpr static auto ntts = std::make_tuple(ntt_builder.build<Mods>()...);
    constexpr static std::size_t mod_cnt = sizeof...(Mods);
    std::array<ll, (1ll << MaxSizeLog)> buf;

    template <std::size_t Index, typename ...ModInts>
    ll garner_init(ll mod, ll idx, ModInts... mod_ints) {
        if constexpr (Index == mod_cnt) {
            return math::garner(mod, mod_ints...);
        } else {
            return gerner_init<Index + 1>(mod, idx, std::get<Index>(ntts).get_result()[idx], mod_ints...);
        }
    }

    template <std::size_t Index, typename Container1, typename Container2>
    void convolution(const Container1 &arr_a, const Container2 &arr_b) {
        if constexpr (Index == mod_cnt) {
            return;
        } else {
            std::get<Index>(ntts).convolution(arr_a, arr_b);
            convolution<Index + 1>(arr_a, arr_b);
        }
    }

    ll garner_calc(ll mod, ll idx) {
        return garner_init<0>(mod, idx);
    }

public:
    using poly = decltype(buf);

    template <typename Container1, typename Container2>
    const poly& convolution(const Container1 &arr_a, const Container2 &arr_b, ll mod) {
        convolution<0>(arr_a, arr_b);
        for (ll i = 0; i < buf.size(); i++) {
            buf[i] = garner_calc(mod, i);
        }
        return buf;
    }
};
