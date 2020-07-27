#pragma once
#include "../util/template.hpp"

namespace math {

template <typename T = ll>
struct fraction {
    T num, den;
    bool neg;

    constexpr fraction(T num, T den)
        : fraction(abs(num), abs(den), gcd(abs(num), abs(den)), (num < 0) ^ (den < 0)) { }

    constexpr fraction(T num_) : fraction(num_, 1) { }

    constexpr fraction() : fraction(0, 1) { }

    constexpr fraction inv() const {
        assert(num);
        return fraction(den, num, 1, neg);
    }

    constexpr fraction operator-() const {
        auto ret = *this;
        ret.neg ^= true;
        return ret;
    }

    constexpr fraction& operator+=(const fraction &rhs) {
        T l = lcm(den, rhs.den);
        T a = num * (l / den);
        T b = rhs.num * (l / rhs.den);
        if (neg) a *= -1;
        if (rhs.neg) b *= -1;
        den = l;
        num = a + b;
        neg = (num < 0);
        num = abs(num);
        reduction();
        return *this;
    }

    constexpr fraction& operator-=(const fraction &rhs) {
        return (*this) += (-rhs);
    }

    constexpr fraction& operator*=(const fraction &rhs) {
        neg ^= rhs.neg;
        num *= rhs.num;
        den *= rhs.den;
        reduction();
        return *this;
    }

    constexpr fraction& operator/=(const fraction &rhs) {
        return (*this) *= rhs.inv();
    }

    constexpr bool operator==(const fraction &rhs) const {
        return num == rhs.num &&
               den == rhs.den &&
               neg == rhs.neg;
    }

    constexpr bool operator<(const fraction &rhs) const {
        T lv = num * rhs.den;
        T rv = rhs.num * den;
        if (neg) lv *= -1;
        if (rhs.neg) rv *= -1;
        return lv < rv;
    }

    constexpr fraction operator+(const fraction &rhs) const { return fraction(*this) += rhs; }
    constexpr fraction operator-(const fraction &rhs) const { return fraction(*this) -= rhs; }
    constexpr fraction operator*(const fraction &rhs) const { return fraction(*this) *= rhs; }
    constexpr fraction operator/(const fraction &rhs) const { return fraction(*this) /= rhs; }
    constexpr bool operator!=(const fraction &rhs) const { return !((*this) == rhs); }
    constexpr bool operator<=(const fraction &rhs) const { return (*this) == rhs || (*this) < rhs; }
    constexpr bool operator>=(const fraction &rhs) const { return !((*this) < rhs); }
    constexpr bool operator>(const fraction &rhs) const { return !((*this) <= rhs); }

    constexpr T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }
    constexpr T lcm(T a, T b) { return a / gcd(a, b) * b; }
    constexpr T abs(T n) { return n < 0 ? -n : n; }

private:
    constexpr void reduction() {
        if (num == 0) {
            neg = false;
            den = 1;
            return;
        }
        auto g = gcd(num, den);
        if (g) {
            num /= g; 
            den /= g;
        }
    }

    constexpr fraction(T num, T den, T g, bool neg) : num(num / g), den(den / g), neg(neg) { }
};

template <typename T>
std::ostream& operator<<(std::ostream &os, fraction<T> f) {
    if (f.neg) os << "-";
    os << f.num;
    os << "/";
    os << f.den;
    return os;
}

}
