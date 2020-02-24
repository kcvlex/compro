#include "../util/template.cpp"

namespace math {

template <ll Mod>
struct modint {

    constexpr modint(ll x) : x(x) { }
    constexpr modint() : modint(0) { }
    constexpr static modint<Mod> add_id_ele() { return modint<Mod>(0); }
    constexpr static modint<Mod> mul_id_ele() { return modint<Mod>(1); }
    constexpr ll& value() { return x; }
    constexpr const ll value() const { return x; }
    
    constexpr modint& operator +=(const modint &oth) {
        x += oth.value();
        if (Mod < x) x -= Mod;
        return *this;
    }

    constexpr modint& operator -=(const modint &oth) {
        x += Mod - oth.value();
        if (Mod < x) x -= Mod;
        return *this;
    }

    constexpr modint& operator *=(const modint &oth) {
        x *= oth.value();
        x %= Mod;
        return *this;
    }

    constexpr modint& operator /=(const modint &oth) {
        (*this) *= oth.inv();
        return *this;
    }

    constexpr modint operator +(const modint &oth) const {
        return modint(x) += oth;
    }

    constexpr modint operator -(const modint &oth) const {
        return modint(x) -= oth;
    }

    constexpr modint operator *(const modint &oth) const {
        return modint(x) *= oth;
    }

    constexpr modint operator /(const modint &oth) const {
        return modint(x) /= oth;
    }

    constexpr modint operator -() const {
        return modint((x != 0) * (Mod - x)); 
    }

    template <typename T>
    constexpr typename std::enable_if<std::is_integral<T>::value, const modint&>::type
    operator =(T t) { (*this) = modint(t); return *this; }

    constexpr modint inv() const {
        return pow(*this, Mod - 2);
    }

private:
    ll x;
};

}
