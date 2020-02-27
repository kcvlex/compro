#pragma once
#include "../util/template.cpp"
#include "base.cpp"

namespace math {

struct DynModint {

    DynModint(ll x, ll mod) : x(x % mod), mod__(mod) { }
    
    DynModint(ll mod) : DynModint(0, mod) { }
    
    ll value() const {
        return x; 
    }

    DynModint add_id_ele() const {
        return DynModint(0, mod__);
    }

    DynModint mul_id_ele() const {
        return DynModint(1, mod__);
    }

    DynModint& operator +=(const DynModint &oth) {
        x += oth.value();
        if (mod() < x) x -= mod();
        return *this;
    }

    DynModint& operator -=(const DynModint &oth) {
        x += mod() - oth.value();
        if (mod() < x) x -= mod();
        return *this;
    }

    DynModint& operator *=(const DynModint &oth) {
        x *= oth.value();
        x %= mod();
        return *this;
    }

    DynModint& operator /=(const DynModint &oth) {
        (*this) *= oth.inv();
        return *this;
    }

    DynModint operator +(const DynModint &oth) const {
        return DynModint(x) += oth;
    }

    DynModint operator -(const DynModint &oth) const {
        return DynModint(x) -= oth;
    }

    DynModint operator *(const DynModint &oth) const {
        return DynModint(x) *= oth;
    }

    DynModint operator /(const DynModint &oth) const {
        return DynModint(x) /= oth;
    }

    DynModint operator -() const {
        return DynModint((x != 0) * (mod() - x)); 
    }

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, const DynModint&>::type
    operator =(T t) {
        (*this) = DynModint(std::forward<T>(t), mod());
        return *this;
    }

    DynModint inv() const {
        return ::math::pow(*this, mod() - 2);
    }

    ll mod() const {
        return mod__;
    }

private:
    ll x, mod__;
};

}
