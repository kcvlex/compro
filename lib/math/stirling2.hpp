#include "ntt.hpp"

namespace math {

template <typename Mult, typename Fact>
vec<Mult::mint> stirling2(Mult &mult, Fact &fa, size_type n) {
    vec<Mult::mint> av(n), bv(n), ans(2 * n - 1);
    for (size_type i = 0; i < n; i++) {
        av[i] = pow(Mult::mint(i), n) * fa.ifact(i);
        bv[i] = Mult::mint(i & 1 ? -1 : 1) * fa.ifact(i);
    }
    mult.multiply(ALL(av), ALL(bv), ans.begin());
    ans.resize(n);
    return ans;
}

}
