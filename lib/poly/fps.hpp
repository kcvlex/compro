#pragma once
#include "conv-base.hpp"

namespace poly {

template <typename T, typename ConvolutionImpl>
struct FPS : vec<T> {
    using value_type = T;
    using vec<T>::vec;
    using conv_type = convolution_interface<T, ConvolutionImpl>;

    static conv_type** get_conv() {
        static conv_type *conv = nullptr;
        return &conv;
    }

    static void set_conv(conv_type *conv) {
        *(get_conv()) = conv;
    }

    size_type degree() const {
        return size_type(this->size()) - 1;
    }

    FPS& reverse() {
        std::reverse(ALL(*this));
        return *this;
    }

    FPS& extend(size_type deg) {
        this->resize(deg + 1);
        return *this;
    }
    
    FPS& prefix(size_type deg) {
        this->resize(deg + 1);
        return *this;
    }

    FPS& shift(size_type s) {
        size_type deg = degree();
        for (size_type i = 0; i + s <= deg; i++) {
            (*this)[i] = (*this)[i + s];
        }
        return prefix(deg - s);
    }

    FPS& middle(size_type l, size_type r) {
        return this->shift(l).prefix(r - l);
    }

    FPS& add(const FPS &rhs) {
        if (degree() < rhs.degree()) extend(rhs.degree());
        for (size_type i = 0; i <= degree(); i++) (*this)[i] += rhs[i];
        return *this;
    }

    FPS& sub(const FPS &rhs) {
        if (degree() < rhs.degree()) extend(rhs.degree());
        for (size_type i = 0; i <= degree(); i++) (*this)[i] -= rhs[i];
        return *this;
    }

    FPS& mul(FPS rhs) {
        (*get_conv())->multiply(*this, std::move(rhs));
        return *this;
    }

    FPS& mul(T t) {
        for (auto &e : (*this)) e *= t;
        return *this;
    }

    FPS& shrink() {
        while (1 <= degree() && this->back() == T(0)) this->pop_back();
        return *this;
    }

    // f(0) != 0
    FPS& inv(size_type deg) {
        FPS orig(*this);
        prefix(0);
        (*this)[0] = (*this)[0].inv();
        for (size_type i = 1; i <= deg; i *= 2) {
            FPS d(i * 2);
            for (size_type j = 0; j <= std::min(d.degree(), orig.degree()); j++) d[j] = orig[j];
            d.mul(*this).mul(*this).prefix(i * 2 - 1);
            this->mul(T(2))
                 .sub(std::move(d))
                 .prefix(i * 2 - 1);
        }
        return prefix(deg);
    }

    // den[0] != 0
    FPS& quo(FPS den) {
        if (degree() < den.degree()) {
            this->clear();
            this->push_back(T(0));
            return *this;
        }
        size_type sz = degree() - den.degree();
        den.reverse().inv(sz);
        return this->reverse()
                    .prefix(sz)
                    .mul(std::move(den))
                    .prefix(sz)
                    .reverse();
    }

    // den[0] != 0
    FPS& mod(FPS den) {
        auto q = *this;
        q.quo(den).mul(std::move(den));
        return this->sub(std::move(q))
                    .shrink();
    }

    FPS& diff() {
        for (ll i = 0; i + 1 <= degree() ; i++) (*this)[i] = (*this)[i + 1] * T(i + 1);
        if (1 <= degree()) prefix(degree() - 1);
        return *this;
    }
    
    FPS& integrate() {
        this->resize(this->size() + 1);
        for (ll i = degree(); 1 <= i; i--) (*this)[i] = (*this)[i - 1] * inv(i);
        (*this)[0] = 0;
        return *this;
    }
    
    template <typename Container>
    FPS& multiply_sparse(Container terms) {
        (*get_conv())->multiply_sparse(*this, terms);
        return *this;
    }

    template <typename Container>
    FPS& divide_sparse(Container terms) {
        (*get_conv())->divide_sparse(*this, terms);
        return *this;
    }

};

template <typename Poly> Poly prefix(Poly f, size_type sz) { return f.prefix(sz); }
template <typename Poly> Poly inv(Poly f, size_type sz) { return f.inv(sz); }
template <typename Poly> Poly add(Poly lhs, const Poly &rhs) { return lhs.add(rhs); }
template <typename Poly> Poly sub(Poly lhs, const Poly &rhs) { return lhs.sub(rhs); }
template <typename Poly> Poly mul(Poly lhs, const Poly &rhs) { return lhs.mul(rhs); }
template <typename Poly> Poly quo(Poly lhs, const Poly &rhs) { return lhs.quo(rhs); }
template <typename Poly> Poly mod(Poly lhs, const Poly &rhs) { return lhs.mod(rhs); }

template <typename Poly> Poly mul(Poly lhs, Poly &&rhs) { return lhs.mul(std::move(rhs)); }
template <typename Poly> Poly quo(Poly lhs, Poly &&rhs) { return lhs.quo(std::move(rhs)); }
template <typename Poly> Poly mod(Poly lhs, Poly &&rhs) { return lhs.mod(std::move(rhs)); }

template <typename Poly1, typename Poly2>
std::pair<Poly1, Poly1> div(Poly1 lhs, Poly2 &&rhs) {
    auto q = std::move(quo(lhs, rhs));
    auto m = std::move(mul(q, std::forward<Poly2>(rhs)));
    lhs.sub(std::move(m)).shrink();
    return std::make_pair(std::move(q), std::move(lhs));
}

// http://q.c.titech.ac.jp/docs/progs/polynomial_division.html
// [x^N](P/Q)
template <typename Poly>
typename Poly::value_type 
coef_of_div(Poly P, Poly Q, size_type N) {
    assert(Q[0] == 1);
    assert(P.size() + 1 == Q.size());

    for (; N; N /= 2) {
        FPS Qn = Q;
        for (size_type i = 1; i < Q.size(); i += 2) Qn[i] *= -1;
        auto PQ = std::move(mul(P, Qn));
        auto QQ = std::move(mul(Q, Qn));
        for (size_type i = 0, offset = N % 2; i < P.size(); i++) P[i] = PQ[2 * i + offset];
        for (size_type i = 0; i < Q.size(); i++) Q[i] = QQ[2 * i];
    }

    return P[0];
}

}
