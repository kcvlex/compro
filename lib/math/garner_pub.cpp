#include <iostream>
#include <cstddef>
#include <tuple>
#include <type_traits>

using ll = std::int64_t;

namespace compro {

template <typename T>
constexpr T pow(const T &n, ll k) {
    T ret = T::mul_id_ele();
    T cur = n;
    while (k) {
        if (k & 1) ret *= cur;
        cur *= cur;
        k /= 2;
    }
    return ret;
}

template <ll Mod>
struct Modint {

    constexpr Modint(ll x) : x(x) { }
    
    constexpr Modint() : Modint(0) { }
    
    constexpr static Modint<Mod> add_id_ele() { 
        return Modint<Mod>(0); 
    }
    
    constexpr static Modint<Mod> mul_id_ele() { 
        return Modint<Mod>(1); 
    }
    
    constexpr ll& value() { 
        return x; 
    }
    
    constexpr ll value() const {
        return x; 
    }

    constexpr Modint& operator +=(const Modint &oth) {
        x += oth.value();
        if (Mod < x) x -= Mod;
        return *this;
    }

    constexpr Modint& operator -=(const Modint &oth) {
        x += Mod - oth.value();
        if (Mod < x) x -= Mod;
        return *this;
    }

    constexpr Modint& operator *=(const Modint &oth) {
        x *= oth.value();
        x %= Mod;
        return *this;
    }

    constexpr Modint& operator /=(const Modint &oth) {
        (*this) *= oth.inv();
        return *this;
    }

    constexpr Modint operator +(const Modint &oth) const {
        return Modint(x) += oth;
    }

    constexpr Modint operator -(const Modint &oth) const {
        return Modint(x) -= oth;
    }

    constexpr Modint operator *(const Modint &oth) const {
        return Modint(x) *= oth;
    }

    constexpr Modint operator /(const Modint &oth) const {
        return Modint(x) /= oth;
    }

    constexpr Modint operator -() const {
        return Modint((x != 0) * (Mod - x)); 
    }

    template <typename T>
    constexpr typename std::enable_if<std::is_integral<T>::value, const Modint&>::type
    operator =(T t) { (*this) = Modint(t); return *this; }

    constexpr Modint inv() const {
        return ::compro::pow(*this, Mod - 2);
    }

    constexpr ll mod() const {
        return Mod;
    }

private:
    ll x;
};

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
    struct Apply__ {
        const Modint<Mod> &v;

        Apply__(const Modint<Mod> &v) : v(v) { }

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
            Apply__ applyer(v);
            applyer.apply(tail...);
            return garner_priv(std::forward<Tail>(tail)...);
        }
    }

};

}

/*
 * Example
 */

constexpr ll mods[] = { 1009, 9995147, 9997451, 1000000007, };
constexpr ll mod = 1000000009;

int main() {
    ll a, b, c, d;
    std::cin >> a >> b >> c >> d;
    compro::Garner<mod, mods[0], mods[1], mods[2], mods[3]> g;
    std::cout << g.garner(a, b, c, d) << std::endl;
    return 0;
}

