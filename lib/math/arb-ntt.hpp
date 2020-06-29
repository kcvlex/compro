#pragma once
#include "ntt.hpp"
#include "garner.hpp"

namespace math {

namespace helper {

template <ll Mod, ll... Mods>
class ntts_type {
    using head_type = std::tuple<NTT<Mod>>;
    using tail_type = typename ntts_type<Mods...>::type;
public:
    using type = decltype(tuple_cat(std::declval<head_type>(),
                                    std::declval<tail_type>()));
};

template <ll Mod>
class ntts_type<Mod> {
public:
    using type = std::tuple<NTT<Mod>>;
};

}

template <ll Mod, ll... Mods>
struct ArbitraryNTT : convolution<ArbitraryNTT<Mod, Mods...>> {
    constexpr ArbitraryNTT() : ntts() { }

    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    void multiply(InputIterator1 begin1, InputIterator1 end1,
                  InputIterator2 begin2, InputIterator2 end2,
                  OutputIterator out) 
    {
        size_type n = std::distance(begin1, end1);
        size_type m = std::distance(begin2, end2);

        call_multiply<0>(begin1, end1, begin2, end2);

        std::array<ll, sizeof...(Mods)> mr;
        for (size_type i = 0; i < n + m - 1; i++) {
            collect_vals<0>(mr, i);
            ll res = garner<Mod, Mods...>(mr);
            *(out + i) = res;
        }
    }

private:
    using ntts_type = typename helper::ntts_type<Mods...>::type;
    ntts_type ntts;

    template <size_type Idx, typename InputIterator1, typename InputIterator2>
    void call_multiply(InputIterator1 begin1, InputIterator1 end1,
                       InputIterator2 begin2, InputIterator2 end2)
    {
        if constexpr (Idx == sizeof...(Mods)) {
        } else {
            std::get<Idx>(ntts).multiply(begin1, end1, begin2, end2);
            call_multiply<Idx + 1>(begin1, end1, begin2, end2);
        }
    }

    template <size_type Idx>
    void collect_vals(std::array<ll, sizeof...(Mods)> &mr, const size_type i) {
        if constexpr (Idx == sizeof...(Mods)) {
        } else {
            mr[Idx] = (std::get<Idx>(ntts).get_last() + i)->value();
            collect_vals<Idx + 1>(mr, i);
        }
    }
};

}
