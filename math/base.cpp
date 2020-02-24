#include "../util/template.cpp"

namespace math {

template <typename T>
T pow(const T &n, ll k) {
    T ret = T();
    T cur = n;
    while (k) {
        if (k & 1) ret *= cur;
        cur *= cur;
        k /= 2;
    }
    return ret;
}

template <typename T, T N, ll K>
constexpr pow() {
    if constexpr (K == 0) {
        return T();
    } else {
        return (K & 1 ? N : T()) * pow<T, N * N, K / 2>();
    }
}

}
