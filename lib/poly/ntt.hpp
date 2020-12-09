#pragma once
#include "util/template.hpp"
#include "math/modint.hpp"
#include "conv-base.hpp"
#include "util/debug.hpp"

namespace poly {

namespace internal {

template <std::uint32_t Mod>
constexpr bool is_primitive_root(std::uint32_t r) {
    math::Modint<Mod> mr(r);
    for (std::uint32_t d = 2; d * d <= Mod; d++) {
        if ((Mod - 1) % d == 0) {
            if (pow(mr, d) == 1) return false;
            if (pow(mr, (Mod - 1) / d) == 1) return false;
        }
    }
    return true;
}

template <std::uint32_t Mod>
constexpr std::uint32_t find_primitive_root(std::uint32_t r) {
    return (is_primitive_root<Mod>(r) ? r : find_primitive_root<Mod>(r + 1));
}

template <std::uint32_t Mod>
constexpr std::uint32_t find_primitive_root() {
    return find_primitive_root<Mod>(2);
}

constexpr auto calc_max_base(std::uint32_t m) {
    std::uint32_t ret = 0;
    for (; m % 2 == 0; ret++, m /= 2);
    return ret;
}

}

template <std::uint32_t Mod, std::uint32_t PrimitiveRoot = internal::find_primitive_root<Mod>()>
struct NTT : convolution_interface<math::Modint<Mod>, NTT<Mod, PrimitiveRoot>> {
    using mint = math::Modint<Mod>;
    using value_type = mint;

    constexpr NTT() 
        :  root_lis(max_size_log), 
           iroot_lis(max_size_log)
    {
        for (std::uint32_t i = 0; i < root_lis.size(); i++) {
            root_lis[i] = pow(mint(PrimitiveRoot), (Mod - 1) >> (i + 2)) * -1;
            iroot_lis[i] = root_lis[i].inv();
        }
    }

    void multiply(vec<value_type> &a, vec<value_type> b) {
        size_type m = a.size(), n = b.size();
        size_type sz = 1, res_sz = n + m - 1;
        while (sz < res_sz) sz *= 2;
        a.resize(sz); b.resize(sz);
        ntt(a, sz);
        ntt(b, sz);
        auto isz = mint(sz).inv();
        for (size_type i = 0; i < sz; i++) a[i] *= b[i] * isz;
        intt(a, sz);
        a.resize(res_sz);
    }

private:
    static constexpr size_type max_size_log = internal::calc_max_base(Mod - 1);
    static constexpr size_type max_size = 1ll << max_size_log;
    static constexpr size_type max_conv_size = max_size * 2;

    vec<mint> root_lis, iroot_lis;
    using uint = std::uint32_t;

    void ntt(vec<value_type> &v, const uint sz) const noexcept {
        if (sz == 1) return;
        if (sz == 2) {
            const mint a = v[0], b = v[1];
            v[0] = a + b;
            v[1] = a - b;
            return;
        }

        const mint r4 =  root_lis[0];
        for (uint m = sz / 4; m; m >>= 2) {
            mint z = 1;
            for (uint s = 0, k = 0; s < v.size(); s += (m << 2)) {
                const mint z0 = 1, 
                           z1 = z,
                           z2 = z1 * z1, 
                           z3 = z2 * z1;
                uint idx0 = s,
                     idx1 = idx0 + m,
                     idx2 = idx1 + m,
                     idx3 = idx2 + m;
                for (; idx0 < s + m; idx0++, idx1++, idx2++, idx3++) {
                    const mint x0 = v[idx0] * z0, x1 = v[idx1] * z1,
                               x2 = v[idx2] * z2, x3 = v[idx3] * z3;
                    const mint x_0p2 = (x0 + x2),
                               x_1p3 = (x1 + x3),
                               x_0m2 = (x0 - x2),
                               x_1m3 = (x1 - x3) * r4;
                    v[idx0] = (x_0p2 + x_1p3);
                    v[idx1] = (x_0p2 - x_1p3);
                    v[idx2] = (x_0m2 + x_1m3);
                    v[idx3] = (x_0m2 - x_1m3);
                }
                z *= root_lis[__builtin_ctz(++k)];
                z *= root_lis[__builtin_ctz(++k)];
            }
            
            if (m == 2) {
                mint z = 1;
                for (uint i = 0, k = 0; i < sz; i += 2) {
                    const mint a = v[i], b = v[i + 1] * z;
                    v[i] = a + b;
                    v[i + 1] = a - b;
                    z *= root_lis[__builtin_ctz(++k)];
                }
            }
        }
    }

    void intt(vec<value_type> &v, size_type sz) const noexcept {
        if (sz == 1) return;
        if (sz == 2) {
            const mint a = v[0], b = v[1];
            v[0] = a + b;
            v[1] = a - b;
            return;
        }

        const mint r4 =  iroot_lis[0];
        for (uint m = 1; m < sz; m <<= 2) {
            if (m * 2 == sz) {
                for (uint i = 0; i < m; i++) {
                    const mint a = v[i], b = v[i + m];
                    v[i] = a + b;
                    v[i + m] = a - b;
                }
                break;
            }

            mint z = 1;
            for (uint s = 0, k = 0; s < v.size(); s += (m << 2)) {
                const mint z0 = 1,
                           z1 = z,
                           z2 = z1 * z1, 
                           rz1 = z1 * r4;
                uint idx0 = s,
                     idx1 = idx0 + m,
                     idx2 = idx1 + m,
                     idx3 = idx2 + m;
                for (; idx0 < s + m; idx0++, idx1++, idx2++, idx3++) {
                    const mint x0 = v[idx0], x1 = v[idx1],
                               x2 = v[idx2], x3 = v[idx3];
                    const mint x_0p1 = (x0 + x1),
                               x_2p3 = (x2 + x3),
                               x_0m1 = (x0 - x1) * z1,
                               x_2m3 = (x2 - x3) * rz1;
                    v[idx0] = (x_0p1 + x_2p3);
                    v[idx1] = (x_0m1 + x_2m3);
                    v[idx2] = (x_0p1 - x_2p3) * z2;
                    v[idx3] = (x_0m1 - x_2m3) * z2;
                }
                z *= iroot_lis[__builtin_ctz(++k)];
                z *= iroot_lis[__builtin_ctz(++k)];
            }
        }
    }
};

}
