#include "../util/template.cpp"

namespace math {

template <typename T>
constexpr T pow(const T &n, ll k) {
    T ret = T::mul_id_ele();
    T cur = n;
    while (k) {
        if (k & 1) ret *= cur;
        cur *= cur;
        k /= 2;
    }
    return ret;
}

}
