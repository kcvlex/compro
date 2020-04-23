#pragma once
#include "../util/template.cpp"
#include "base.cpp"

namespace math {

template <ll Mod>
struct Modint {

    constexpr Modint(ll x) noexcept : x((Mod + x % Mod) % Mod) { }
    
    constexpr Modint() noexcept : Modint(0) { }
    
    constexpr Modint<Mod> add_id_ele() const noexcept { 
        return Modint<Mod>(0); 
    }
    
    constexpr Modint<Mod> mul_id_ele() const noexcept {
        return Modint<Mod>(1); 
    }
    
    constexpr ll& value() noexcept { 
        return x; 
    }
    
    constexpr ll value() const noexcept {
        return x; 
    }

    constexpr Modint& operator +=(const Modint &oth) noexcept {
        x += oth.value();
        if (Mod <= x) x -= Mod;
        return *this;
    }

    constexpr Modint& operator -=(const Modint &oth) noexcept {
        x += Mod - oth.value();
        if (Mod <= x) x -= Mod;
        return *this;
    }

    constexpr Modint& operator *=(const Modint &oth) noexcept {
        x *= oth.value();
        x %= Mod;
        return *this;
    }

    constexpr Modint& operator /=(const Modint &oth) noexcept {
        x *= oth.inv();
        x %= Mod;
        return *this;
    }

    constexpr Modint operator +(const Modint &oth) const noexcept {
        return Modint(x) += oth;
    }

    constexpr Modint operator -(const Modint &oth) const noexcept {
        return Modint(x) -= oth;
    }

    constexpr Modint operator *(const Modint &oth) const noexcept {
        return Modint(x) *= oth;
    }

    constexpr Modint operator /(const Modint &oth) const noexcept {
        return Modint(x) /= oth;
    }

    constexpr Modint operator -() const noexcept {
        return Modint((x != 0) * (Mod - x)); 
    }

    template <typename T>
    constexpr typename std::enable_if<std::is_integral<T>::value, const Modint&>::type
    operator =(T t) noexcept {
        (*this) = Modint(std::forward<T>(t)); 
        return *this;
    }

    constexpr Modint inv() const noexcept {
        return ::math::pow(*this, Mod - 2);
    }

    constexpr ll mod() const noexcept {
        return Mod;
    }

private:
    ll x;
};

}
