#pragma once
#include "../util/template.cpp"

namespace math {

constexpr ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }

struct fraction {
    ll num, den;  // WARNING : den == 0
    bool sgn_;  // 0 : plus, 1 : minus

    fraction(ll n, ll d) {
        sgn_ = (n * d < 0);
        n = std::abs(n);
        d = std::abs(d);
        if (n && d) {
            ll g = gcd(n, d);
            num = n / g;
            den = d / g;
        } else {
            num = 0, den = 0;
            if (n) num = n / n;
            if (d) den = d / d;
        }
    }

    constexpr int sgn() const {
        return sgn_ ? -1 : 1;
    }

    constexpr bool operator ==(const fraction &oth) const {
        return num == oth.num && den == oth.den && sgn() == oth.sgn();
    }

    constexpr bool operator !=(const fraction &oth) const {
        return !(*this == oth);
    }

    constexpr bool operator <(const fraction &oth) const {
        ll a = num * oth.den;
        ll b = oth.num * den;
        return a * sgn() < b * oth.sgn();
    }

    constexpr bool operator <=(const fraction &oth) const {
        return *this == oth || *this < oth;
    }

    constexpr bool operator >(const fraction &oth) const {
        return !(*this <= oth);
    }

    constexpr bool operator >=(const fraction &oth) const {
        return !(*this < oth);
    }

    constexpr long double value() const {
        return static_cast<long double>(num) / static_cast<long double>(den) * sgn();
    }

    fraction& inv() {
        std::swap(num, den);
        return *this;
    }

    fraction inv() const {
        return fraction(*this).inv();
    }

    constexpr fraction& operator -() {
        sgn_ = !sgn_;
        return *this;
    }

    fraction operator -() const {
        return -fraction(*this);
    }

    fraction& operator +=(const fraction &oth) {
        ll g = gcd(den, oth.den);
        ll n = oth.den / g * num * sgn() +
               den / g * oth.num * oth.sgn();
        *this = fraction(n, den / g * oth.den);
        return *this;
    }

    fraction& operator -=(const fraction &oth) {
        return (*this) += (-oth);
    }

    fraction& operator *=(const fraction &oth) {
        ll n = num * oth.num * sgn() * oth.sgn();
        ll d = den * oth.den;
        *this = fraction(n, d);
        return *this;
    }

    fraction& operator /=(const fraction &oth) {
        return (*this) *= oth.inv();
    }

    fraction operator +(const fraction &oth) const {
        return fraction(*this) += oth;
    }

    fraction operator -(const fraction &oth) const {
        return fraction(*this) -= oth;
    }

    fraction operator *(const fraction &oth) const {
        return fraction(*this) *= oth;
    }

    fraction operator /(const fraction &oth) const {
        return fraction(*this) /= oth;
    }
};

}
