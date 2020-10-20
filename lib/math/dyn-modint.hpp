#pragma once
#include "base.hpp"

namespace math {

template <const ll &Mod>
struct DynModint {

    DynModint(ll x) noexcept : x((Mod + x % Mod) % Mod) { }
    
    DynModint() noexcept : DynModint(0) { }
    
    static DynModint add_id_ele() { 
        return DynModint(0); 
    }
    
    static DynModint mul_id_ele() {
        return DynModint(1); 
    }
    
    ll& value() noexcept { 
        return x; 
    }
    
    ll value() const noexcept {
        return x; 
    }

    DynModint& operator+=(const DynModint &oth) noexcept {
        x += oth.value();
        if (Mod <= x) x -= Mod;
        return *this;
    }

    DynModint& operator-=(const DynModint &oth) noexcept {
        x += Mod - oth.value();
        if (Mod <= x) x -= Mod;
        return *this;
    }

    DynModint& operator*=(const DynModint &oth) noexcept {
        x *= oth.value();
        x %= Mod;
        return *this;
    }

    DynModint& operator/=(const DynModint &oth) noexcept {
        x *= oth.inv().value();
        x %= Mod;
        return *this;
    }

    DynModint operator+(const DynModint &oth) const noexcept {
        return DynModint(x) += oth;
    }

    DynModint operator-(const DynModint &oth) const noexcept {
        return DynModint(x) -= oth;
    }

    DynModint operator*(const DynModint &oth) const noexcept {
        return DynModint(x) *= oth;
    }

    DynModint operator/(const DynModint &oth) const noexcept {
        return DynModint(x) /= oth;
    }

    DynModint operator-() const noexcept {
        return DynModint((x != 0) * (Mod - x)); 
    }

    bool operator==(const DynModint &oth) const noexcept {
        return value() == oth.value();
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, const DynModint&>::type
    operator=(T t) noexcept {
        (*this) = DynModint(std::forward<T>(t)); 
        return *this;
    }

    DynModint inv() const noexcept {
        return ::math::pow(*this, Mod - 2);
    }

    ll mod() const noexcept {
        return Mod;
    }

private:
    ll x;
};

template <const ll &Mod>
DynModint<Mod> inv(DynModint<Mod> m) {
    m.inv();
    return m;
}

template <const ll &Mod>
std::istream& operator>>(std::istream &is, DynModint<Mod> &m) {
    ll v;
    is >> v;
    m = v;
    return is;
}

template <const ll &Mod>
std::ostream& operator<<(std::ostream &os, const DynModint<Mod> &m) {
    os << m.value();
    return os;
}

}
