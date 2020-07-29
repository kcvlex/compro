#include "ntt.hpp"

namespace math {

template <typename Conv, typename Fact>
vec<Conv::value_type> stirling2(Mult &mult, Fact &fa, size_type n) {
    static_assert(std::is_base_of<convolution, Conv>::value, "convolution isn't base of Conv");
    using value_type = Conv::value_type;

    vec<value_type> av(n), bv(n), ans(2 * n - 1);
    for (size_type i = 0; i < n; i++) {
        av[i] = pow(value_type(i), n) * fa.ifact(i);
        bv[i] = value_type(i & 1 ? -1 : 1) * fa.ifact(i);
    }
    mult.multiply(ALL(av), ALL(bv), ans.begin());
    ans.resize(n);
    return ans;
}

}
