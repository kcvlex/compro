#pragma once
#include "util/template.hpp"
#include "base.hpp"

namespace math {

template <ull Mod>
struct Modint {

    constexpr Modint(ll x) noexcept : x((Mod + x % static_cast<ll>(Mod)) % Mod) { }
    constexpr Modint() noexcept : Modint(0) { }
    
    constexpr static Modint add_id_ele() { 
        return Modint(0); 
    }
    
    constexpr static Modint mul_id_ele() {
        return Modint(1);
    }
    
    constexpr ll value() const noexcept { 
        return static_cast<ll>(x);
    }

    constexpr Modint& operator+=(const Modint &oth) noexcept {
        x += oth.value();
        if (Mod <= x) x -= Mod;
        return *this;
    }

    constexpr Modint& operator-=(const Modint &oth) noexcept {
        x += Mod - oth.value();
        if (Mod <= x) x -= Mod;
        return *this;
    }

    constexpr Modint& operator*=(const Modint &oth) noexcept {
        x *= oth.value();
        x %= Mod;
        return *this;
    }

    constexpr Modint& operator/=(const Modint &oth) noexcept {
        x *= oth.inv().value();
        x %= Mod;
        return *this;
    }

    constexpr Modint operator+(const Modint &oth) const noexcept {
        return Modint(x) += oth;
    }

    constexpr Modint operator-(const Modint &oth) const noexcept {
        return Modint(x) -= oth;
    }

    constexpr Modint operator*(const Modint &oth) const noexcept {
        return Modint(x) *= oth;
    }

    constexpr Modint operator/(const Modint &oth) const noexcept {
        return Modint(x) /= oth;
    }

    constexpr Modint operator-() const noexcept {
        return Modint((x != 0) * (Mod - x)); 
    }

    constexpr bool operator==(const Modint &oth) const noexcept {
        return value() == oth.value();
    }

    template <typename T>
    constexpr typename std::enable_if<std::is_integral<T>::value, const Modint&>::type
    operator=(T t) noexcept {
        (*this) = Modint(std::forward<T>(t)); 
        return *this;
    }

    constexpr Modint inv() const noexcept {
        return ::math::pow(*this, Mod - 2);
    }

    constexpr ull mod() const noexcept {
        return Mod;
    }

private:
    ull x;
};

template <ull Mod>
Modint<Mod> inv(Modint<Mod> m) {
    return m.inv();
}

template <ull Mod>
std::istream& operator>>(std::istream &is, Modint<Mod> &m) {
    ll v;
    is >> v;
    m = v;
    return is;
}

template <ull Mod>
std::ostream& operator<<(std::ostream &os, Modint<Mod> m) {
    os << m.value();
    return os;
}

}
