#pragma once
#include "math/base.hpp"

namespace poly {

template <typename T, typename Impl>
struct convolution_interface {
    void multiply(vec<T> &a, vec<T> b) {
        static_cast<Impl*>(this)->multiply(a, b);
    }

    // TODO : value_type
    template <typename Container>
    void multiply_sparse(vec<T> &a, Container terms) {
        std::sort(ALLR(terms), [&](const auto &p, const auto &q) { return p.second < q.second; });
        a.resize(a.size() + terms[0].second);
        for (size_type i = size_type(a.size() - 1); 0 <= i; i--) {
            auto cur = std::move(a[i]);
            a[i] = T();
            for (const auto &[ v, deg ] : terms) if (i + deg < a.size()) a[i + deg] += cur * v;
        }
    }

    // TODO : buggy ? 
    template <typename Container>
    void divide_sparse(vec<T> &a, Container terms) {
        std::sort(ALL(terms), [&](const auto &p, const auto &q) { return p.second < q.second; });
        assert(terms[0].second == 0);
        for (size_type i = 0; i < size_type(a.size()); i++) {
            a[i] *= terms[0].first;
            for (const auto &[ v, deg ] : terms) if (deg && 0 <= i - deg) a[i] -= v * a[i - deg];
        }
    }
};

template <typename T>
struct SimpleConvolution : convolution_interface<T, SimpleConvolution<T>> {
    void multiply(vec<T> &a, vec<T> b) {
        a.resize(a.size() + b.size()  - 1);
        for (size_type i = size_type(a.size()) - 1; 0 <= i; i--) {
            auto cur = std::move(a[i]);
            a[i] = T();
            for (size_type j = size_type(b.size()); 0 <= j; j--) {
                if (i + j < a.size()) a[i + j] += cur * b[j];
            }
        }
    }
};

}
