#include "../util/template.cpp"
#include "modint.cpp"

namespace math {

template <ll AimMod, ll... OtherMods>
struct Garner {
    template <ll Mod> using Values = std::tuple<Modint<Mod>, Modint<Mod>, Modint<Mod>>;

    Garner() { }

    template <typename ...Args>
    typename std::enable_if<std::is_same<
                    std::tuple_size<decltype(std::make_tuple(OtherMods...))>,
                    std::tuple_size<decltype(std::make_tuple(std::declval<Args>()...))>
                >::value, ll>::type
    garner(Args... args) {
        return apply<mods_size + 1>(make_values<0>(std::make_tuple(args...)));
    }

private:
    static constexpr std::size_t mods_size = sizeof...(OtherMods);
    static constexpr auto oth_mod_tuple = std::make_tuple(OtherMods...);

    template <ll Mod>
    struct Applyer {
        const Modint<Mod> &v;

        Applyer(const Modint<Mod> &v) : v(v) { }

        void apply() { }

        template <typename Head, typename ...Tail>
        void apply(Head &head, Tail&... tail) {
            apply__(head);
            apply(tail...);
        }
       
        template <typename T>
        void apply__(T &val) {
            auto vv = v.value();
            auto vm = decltype(std::get<0>(val))(vv);
            std::get<2>(val) += vm * std::get<1>(val);
            std::get<1>(val) *= v.mod();
        }
    };

    template <std::size_t Index, typename Tuple>
    constexpr auto make_values(Tuple &&t) const {
        if constexpr (Index == mods_size) {
            return std::make_tuple(make_value<AimMod>(0));
        } else {
            return std::tuple_cat(std::make_tuple(make_value<std::get<Index>(oth_mod_tuple)>(std::get<Index>(t))),
                                  make_values<Index + 1, Tuple>(std::forward<Tuple>(t)));
        }
    }
    
    template <ll Mod>
    constexpr Values<Mod> make_value(ll t) const {
        return Values<Mod>(Modint<Mod>(t), Modint<Mod>(1), Modint<Mod>(0));
    }

    template <std::size_t Index, typename Tuple, typename ...Args>
    constexpr ll apply(Tuple &&t, Args&&... args) const {
        if constexpr (Index == 0) {
            return garner_priv(std::forward<Args>(args)...);
        } else {
            return apply<Index - 1>(t, std::get<Index - 1>(t), std::forward<Args>(args)...);
        }
    }

    template <typename Head, typename ...Tail> 
    constexpr ll garner_priv(Head &&head, Tail&&... tail) const {
        if constexpr (sizeof...(tail) == 0) {
            return std::get<2>(head).value();
        } else {
            auto v = (std::get<0>(head) - std::get<2>(head)) * std::get<1>(head).inv();
            Applyer applyer(v);
            applyer.apply(tail...);
            return garner_priv(std::forward<Tail>(tail)...);
        }
    }

};

}
