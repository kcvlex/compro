#pragma once
#include "template.hpp"

namespace utility {

template <typename T>
struct Compress {
    using size_type = ssize_t;

    Compress(vec<T> arg) : v(std::move(arg)) {
        v = std::move(arg);
        std::sort(ALL(v));
        auto ite = std::unique(ALL(v));
        v.erase(ite, v.end());
    }

    const T& operator [](size_type i) const {
        return v[i];
    }

    size_type get(T &&t) const {
        return std::distance(v.begin(),
                             std::lower_bound(ALL(v), t));
    }

    size_type size() const {
        return v.size();
    }

private:
    vec<T> v;
};

};
