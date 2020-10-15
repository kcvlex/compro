#pragma once
#include "ntt.hpp"

namespace math {

template <typename T, typename Convolution>
struct FPS : vec<T> {
    
    using value_type = T;
    using vec<T>::vec;

    static Convolution** get_conv() {
        static Convolution *conv = nullptr;
        return &conv;
    }

    static void set_conv(Convolution *conv) {
        *(get_conv()) = conv;
    }

    size_type degree() const {
        return this->size();
    }

    FPS& operator+=(const FPS &rhs) {
        if (this->size() < rhs.size()) resize(rhs.size());
        for (size_type i = 0; i < this->size(); i++) (*this)[i] += rhs[i];
        return *this;
    }

    FPS& operator-=(const FPS &rhs) {
        if (this->size() < rhs.size()) this->resize(rhs.size());
        for (size_type i = 0; i < degree(); i++) (*this)[i] -= rhs[i];
        return *this;
    }

    FPS& operator*=(const FPS &rhs) {
        (*get_conv())->multiply(*this, rhs);
        return *this;
    }

    FPS& operator*=(const T &rhs) {
        for (auto &e : (*this)) e *= rhs;
        return *this;
    }

    FPS& operator/=(const FPS &rhs) {
        div(rhs);
        return *this;
    }

    FPS& operator/=(const T &rhs) {
        return (*this) *= inv(rhs);
    }

    FPS& operator%=(const FPS &rhs) {
        mod(rhs);
        return *this;
    }

    FPS operator+(const FPS &rhs) const {
        return FPS(*this) += rhs;
    }

    FPS operator-(const FPS &rhs) const {
        return FPS(*this) -= rhs;
    }

    template <typename U>
    FPS operator*(const U &rhs) const {
        return FPS(*this) *= rhs;
    }

    template <typename U>
    FPS operator/(const U &rhs) const {
        return FPS(*this) /= rhs;
    }

    FPS operator%(const FPS &rhs) const {
        return FPS(*this) %= rhs;
    }
    
    void prefix(size_type deg) {
        this->resize(deg);
    }

    // f(0) != 0
    void inv(size_type deg) {
        FPS orig = *this;
        prefix(1);
        (*this)[0] = (*this)[0].inv();
        for (size_type i = 1; i < deg; i *= 2) {
            FPS tmp(i * 2);
            for (size_type j = 0; j < i * 2; j++) tmp[j] = (j < orig.degree() ? orig[j] : T());
            auto dec = (*this) * (*this) * tmp;
            (*this) *= 2;
            (*this) -= dec;
            this->prefix(i * 2);
        }
    }

    // den[0] != 0
    void div(FPS den) {
        if (degree() < den.degree()) return;
        std::reverse(ALL((*this)));
        std::reverse(ALL(den));
        size_type sz = degree() - den.degree() + 1;
        (*this) *= inv(std::move(den), sz);
        prefix(sz);
        std::reverse(ALL((*this)));
    }

    // den[0] != 0
    void mod(FPS den) {
        div(den);
        (*this) -= (*this) * den;
        while (1 < degree() && this->back() == 0) this->pop_back();
    }
};

template <typename T, typename Convolution>
FPS<T, Convolution> prefix(FPS<T, Convolution> f, size_type deg) {
    f.prefix(deg);
    return f;
}

template <typename T, typename Convolution>
FPS<T, Convolution> inv(FPS<T, Convolution> f, size_type deg) {
    f.inv(deg);
    return f;
}

template <typename T, typename Convolution>
FPS<T, Convolution> div(FPS<T, Convolution> num, FPS<T, Convolution> den) {
    num.div(std::move(den));
    return num;
}

template <typename T, typename Convolution>
FPS<T, Convolution> mod(FPS<T, Convolution> num, FPS<T, Convolution> den) {
    num.mod(std::move(den));
    return num;
}

// http://q.c.titech.ac.jp/docs/progs/polynomial_division.html
// [x^N](P/Q)
template <typename T, typename Convolution>
T coef_of_div(FPS<T, Convolution> P, FPS<T, Convolution> Q, size_type N) {
    assert(Q[0] == 1);
    assert(P.size() + 1 == Q.size());

    for (; N; N /= 2) {
        FPS Qn = Q;
        for (size_type i = 1; i < Q.size(); i += 2) Qn[i] *= -1;
        auto PQ = P * Qn;
        auto QQ = Q * Qn;
        for (size_type i = 0, offset = N % 2; i < P.size(); i++) P[i] = PQ[2 * i + offset];
        for (size_type i = 0; i < Q.size(); i++) Q[i] = QQ[2 * i];
    }

    return P[0];
}

}
