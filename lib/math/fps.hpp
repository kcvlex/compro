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

    FPS& operator +=(const FPS &rhs) {
        if (this->size() < rhs.size()) resize(rhs.size());
        for (size_type i = 0; i < this->size(); i++) (*this)[i] += rhs[i];
        return *this;
    }

    FPS& operator -=(const FPS &rhs) {
        if (this->size() < rhs.size()) resize(rhs.size());
        for (size_type i = 0; i < this->size(); i++) (*this)[i] -= rhs[i];
        return *this;
    }

    FPS& operator *=(const FPS &rhs) {
        const size_type sz = this->size() + rhs.size() - 1;
        FPS tmp(sz);
        (*get_conv())->multiply(ALL((*this)), ALL(rhs), tmp.begin());
        return (*this) = std::move(tmp);
    }

    FPS operator +(const FPS &rhs) const {
        return FPS(*this) += rhs;
    }

    FPS operator -(const FPS &rhs) const {
        return FPS(*this) -= rhs;
    }

    FPS operator *(const FPS &rhs) const {
        return FPS(*this) *= rhs;
    }
};

// http://q.c.titech.ac.jp/docs/progs/polynomial_division.html
// [x^N](P/Q)
template <typename FPS>
auto coef_of_div(FPS P, FPS Q, size_type N) {
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
