#pragma once
#include "template.hpp"

namespace utility {

template <typename T>
struct Compressor {
    using size_type = ssize_t;

    Compressor(vec<T> arg) : v(std::move(arg)) {
        std::sort(ALL(v));
        auto ite = std::unique(ALL(v));
        v.erase(ite, v.end());
    }

    const T& operator [](size_type i) const {
        return v[i];
    }

    size_type get(const T &t) const {
        return std::distance(v.begin(),
                             std::lower_bound(ALL(v), t));
    }

    size_type size() const {
        return v.size();
    }

    template <typename U = T>
    vec<U> compress(const vec<T> &v) const {
        vec<U> ret(v.size());
        for (size_type i = 0; i < v.size(); i++) ret[i] = get(v[i]);
        return ret;
    }

private:
    vec<T> v;
};

};
