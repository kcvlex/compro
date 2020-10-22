#pragma once
#include "util/template.hpp"
#include "math/modint.hpp"
#include "conv-base.hpp"

namespace poly {

namespace internal {

template <ull Mod>
constexpr bool is_primitive_root(ll r) {
    math::Modint<Mod> mr(r);
    for (ll d = 2; d * d <= Mod; d++) {
        if ((Mod - 1) % d == 0) {
            if (pow(mr, d).value() == 1) return false;
            if (pow(mr, (Mod - 1) / d).value() == 1) return false;
        }
    }
    return true;
}

template <ull Mod>
constexpr ll find_primitive_root(ll r) {
    return (is_primitive_root<Mod>(r) ? r : find_primitive_root<Mod>(r + 1));
}

template <ull Mod>
constexpr ll find_primitive_root() {
    return find_primitive_root<Mod>(2);
}

constexpr auto calc_max_base(ll m) {
    ll ret = 0;
    for (; m % 2 == 0; ret++, m /= 2);
    return ret;
}

}

template <ull Mod, ull PrimitiveRoot = internal::find_primitive_root<Mod>()>
struct NTT : convolution_interface<math::Modint<Mod>, NTT<Mod, PrimitiveRoot>> {
    using mint = math::Modint<Mod>;
    using value_type = mint;

    constexpr NTT() 
        :  root_lis(max_size_log), 
           iroot_lis(max_size_log)
    {
        for (size_type i = 0; i < static_cast<size_type>(root_lis.size()); i++) {
            root_lis[i] = pow(mint(PrimitiveRoot), (Mod - 1) >> (i + 2)) * -1;
            iroot_lis[i] = root_lis[i].inv();
        }
    }

    void multiply(vec<value_type> &a, vec<value_type> b) {
        size_type m = a.size(), n = b.size();
        size_type sz = 1, res_sz = n + m - 1;
        while (sz < res_sz) sz *= 2;
        a.resize(sz); b.resize(sz);
        ntt(a, sz, false);
        ntt(b, sz, false);
        auto isz = mint(sz).inv();
        for (size_type i = 0; i < sz; i++) a[i] *= b[i] * isz;
        ntt(a, sz, true);
        a.resize(res_sz);
    }

private:
    static constexpr size_type max_size_log = internal::calc_max_base(Mod - 1);
    static constexpr size_type max_size = 1ll << max_size_log;
    static constexpr size_type max_conv_size = max_size * 2;

    vec<mint> root_lis, iroot_lis;
    using buf_iterator = typename vec<mint>::iterator;

    void ntt(vec<value_type> &v, size_type sz, bool inv) {
        if (!inv) {
            for (int m = sz / 2; m; m /= 2) {
                mint mul = 1;
                for(int s = 0, k = 0; s < sz; s += 2 * m) {
                    for(int i = s, j = s + m; i < s + m; ++i, ++j) {
                        auto x = v[i], y = v[j] * mul;
                        v[i] = x + y;
                        v[j] = x - y;
                    }
                    mul *= root_lis[__builtin_ctz(++k)];
                }
            }
        } else {
            for (int m = 1; m < sz; m *= 2) {
                mint mul = 1;
                for (int s = 0, k = 0; s < sz; s += 2 * m) {
                    for (int i = s, j = s + m; i < s + m; i++, j++) {
                        auto l = v[i], r = v[j];
                        v[i] = l + r;
                        v[j] = (l - r) * mul;
                    }
                    mul *= iroot_lis[__builtin_ctz(++k)];
                }
            }
        }
    }
};

}
