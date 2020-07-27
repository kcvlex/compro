#pragma once
#include "../util/template.hpp"
#include "../util/generics.hpp"
#include "modint.hpp"
#include "base.hpp"

namespace math {

namespace ntt_helper {

template <ll Mod>
constexpr bool is_primitive_root(ll r) {
    Modint<Mod> mr(r);
    for (ll d = 2; d * d <= Mod; d++) {
        if ((Mod - 1) % d == 0) {
            if (pow(mr, d).value() == 1) return false;
            if (pow(mr, (Mod - 1) / d).value() == 1) return false;
        }
    }
    return true;
}

template <ll Mod>
constexpr ll find_primitive_root(ll r) {
    return (is_primitive_root<Mod>(r) ? r : find_primitive_root<Mod>(r + 1));
}

template <ll Mod>
constexpr ll find_primitive_root() {
    return find_primitive_root<Mod>(2);
}

constexpr auto calc_max_base(ll m) {
    ll ret = 0;
    for (; m % 2 == 0; ret++, m /= 2);
    return ret;
}

}

template <typename T>
struct convolution {
    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    void multiply(InputIterator1 begin1, InputIterator1 end1,
                  InputIterator2 begin2, InputIterator2 end2,
                  OutputIterator out)
    {
        static_cast<T*>(this)->multiply(begin1, end1, begin2, end2, out);
    }
};

template <ll Mod, ll PrimitiveRoot>
struct ntt__ : convolution<ntt__<Mod, PrimitiveRoot>> {
    using mint = Modint<Mod>;

    constexpr ntt__() 
        :  root_lis(max_size_log), 
           iroot_lis(max_size_log)
    {
        for (size_type i = 0; i < root_lis.size(); i++) {
            root_lis[i] = pow(mint(PrimitiveRoot), (Mod - 1) >> (i + 2)) * -1;
            iroot_lis[i] = root_lis[i].inv();
        }
    }
    
    template <typename InputIterator1, typename InputIterator2>
    auto multiply(InputIterator1 begin1, InputIterator1 end1,
                  InputIterator2 begin2, InputIterator2 end2) 
    {
        size_type n = std::distance(begin1, end1);
        size_type m = std::distance(begin2, end2);
        size_type sz = 1;
        while (sz < n + m - 1) sz *= 2;
        resize_buf(sz);
        std::fill(buf[0].begin(), buf[0].begin() + sz, mint(0));
        std::fill(buf[1].begin(), buf[1].begin() + sz, mint(0));
        std::copy(begin1, end1, buf[0].begin());
        std::copy(begin2, end2, buf[1].begin());
        ntt(ALL(buf[0]), sz, false);
        ntt(ALL(buf[1]), sz, false);
        auto isz = mint(sz).inv();
        for (size_type i = 0; i < sz; i++) buf[0][i] *= buf[1][i] * isz;
        ntt(ALL(buf[0]), sz, true);
        return buf[0];
    }

    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    void multiply(InputIterator1 begin1, InputIterator1 end1,
                  InputIterator2 begin2, InputIterator2 end2,
                  OutputIterator out)
    {
        size_type n = std::distance(begin1, end1);
        size_type m = std::distance(begin2, end2);
        multiply(begin1, end1, begin2, end2);
        std::copy(buf[0].begin(), buf[0].begin() + (n + m - 1), out);
    }

    auto get_last() {
        return buf[0].begin();
    }

    void resize_buf(size_type sz) {
        if (buf[0].size() < sz) {
            buf[0].resize(sz); 
            buf[1].resize(sz);
        }
    }

private:
    static constexpr size_type max_size_log = ntt_helper::calc_max_base(Mod - 1);
    static constexpr size_type max_size = 1ll << max_size_log;
    static constexpr size_type max_conv_size = max_size * 2;

    vec<mint> root_lis, iroot_lis;
    // std::array<mint, max_conv_size> buf[2];
    // using buf_iterator = typename std::array<mint, max_conv_size>::iterator;
    vec<mint> buf[2];
    using buf_iterator = typename vec<mint>::iterator;

    void ntt(buf_iterator begin, buf_iterator end, size_type sz, bool inv) {
        if (!inv) {
            for (int m = sz / 2; m; m /= 2) {
                mint mul = 1;
                for(int s = 0, k = 0; s < sz; s += 2 * m) {
                    for(int i = s, j = s + m; i < s + m; ++i, ++j) {
                        auto x = *(begin + i), y = *(begin + j) * mul;
                        *(begin + i) = x + y;
                        *(begin + j) = x - y;
                    }
                    mul *= root_lis[__builtin_ctz(++k)];
                }
            }
        } else {
            for (int m = 1; m < sz; m *= 2) {
                mint mul = 1;
                for (int s = 0, k = 0; s < sz; s += 2 * m) {
                    for (int i = s, j = s + m; i < s + m; i++, j++) {
                        auto l = *(begin + i), r = *(begin + j);
                        *(begin + i) = l + r;
                        *(begin + j) = (l - r) * mul;
                    }
                    mul *= iroot_lis[__builtin_ctz(++k)];
                }
            }
        }
    }
};

template <ll Mod>
using NTT = ntt__<Mod, ntt_helper::find_primitive_root<Mod>()>;

}
