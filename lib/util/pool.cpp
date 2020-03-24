#include "template.cpp"

template <typename T, std::size_t Size>
struct Pool {
    std::array<T, Size> pool;
    std::size_t pool_idx = 0;

    T& operator [](std::size_t i) {
        return pool[i];
    }

    const T& operator [](std::size_t i) const {
        return pool[i];
    }

    template <typename ...Args>
    std::pair<T&, std::size_t> alloc(Args&&... args) {
        pool[pool_idx] = T(std::forward<Args>(args)...);
        auto idx = pool_idx++;
        return std::make_pair(pool[idx], idx);
    }
};
