#pragma once
#include "../util/template.cpp"
#include "base.cpp"

namespace math {

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
    operator =(T t) {
        (*this) = Modint(std::forward<T>(t)); 
        return *this;
    }

    constexpr Modint inv() const {
        return ::math::pow(*this, Mod - 2);
    }

    constexpr ll mod() const {
        return Mod;
    }

private:
    ll x;
};

}
