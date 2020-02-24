#include "../util/template.cpp"

namespace math {

template <ll Mod>
struct ModInt {

    constexpr ModInt(ll x) : x(x) { this->x %= Mod; }
    constexpr ll& value() { return x; }
    constexpr const ll value() const { return x; }
    
    constexpr ModInt& operator +=(const ModInt &oth) {
        x += oth.value();
        if (Mod < x) x -= Mod;
        return *this;
    }

    constexpr ModInt& operator -=(const ModInt &oth) {
        x += Mod - oth.value();
        if (Mod < x) x -= Mod;
        return *this;
    }

    constexpr ModInt& operator *=(const ModInt &oth) {
        x *= oth.value();
        x %= Mod;
        return *this;
    }

    constexpr ModInt& operator /=(const ModInt &oth) {
        (*this) *= oth.inv();
        return *this;
    }

    constexpr ModInt operator +(const ModInt &oth) const {
        return ModInt(x) += oth;
    }

    constexpr ModInt operator -(const ModInt &oth) const {
        return ModInt(x) -= oth;
    }

    constexpr ModInt operator *(const ModInt &oth) const {
        return ModInt(x) *= oth;
    }

    constexpr ModInt operator /(const ModInt &oth) const {
        return ModInt(x) /= oth;
    }

    constexpr ModInt operator -() const {
        return ModInt((x != 0) * (Mod - x)); 
    }

    template <typename T>
    constexpr typename std::enable_if<std::is_integral<T>::value, const ModInt&>::type
    operator =(T t) { (*this) = ModInt(t); return *this; }

    constexpr ModInt inv() const {
        return pow(*this, Mod - 2);
    }

private:
    ll x;
};

}

template <ll Mod> using mint = math::ModInt<Mod>;
