#include "template.hpp"

namespace utility {

template <typename T>
struct Pool {
    vec<T> pool;

    Pool(std::size_t sz) {
        pool.reserve(sz);
    }

    T& operator [](std::size_t i) {
        return pool[i];
    }

    const T& operator [](std::size_t i) const {
        return pool[i];
    }

    template <typename... Args>
    std::size_t alloc(Args&&... args) {
        ll ret = pool.size();
        pool.emplace_back(std::forward<Args>(args)...);
        return ret;
    }
};

}
