#pragma once
#include "modint.hpp"
#include "../util/generics.hpp"

namespace math {

template <ll Mod, ll... Mods>
ll garner(const std::array<ll, sizeof...(Mods)> &mr_) {
    constexpr size_type sz = sizeof...(Mods) + 1;
    std::array<ll, sz> mr;
    std::array<ll, sz> mods = {{ Mods..., Mod }};
    std::copy(ALL(mr_), mr.begin());
    mr.back() = 0;
    
    std::array<ll, sz> coffs, cnsts;
    std::fill(ALL(coffs), 1);
    std::fill(ALL(cnsts), 0);

    auto add = [](ll a, ll b, ll mod) {
        return (a + b + mod) % mod;
    };

    auto mul = [](ll a, ll b, ll mod) {
        return a * b % mod;
    };

    auto mod_pow = [&](ll n, ll k, ll mod) {
        ll ret = 1, cur = n;
        while (k) {
            if (k & 1) ret = mul(ret, cur, mod);
            cur = mul(cur, cur, mod);
            k /= 2;
        }
        return ret;
    };

    auto mod_inv = [&](ll n, ll mod) {
        return mod_pow(n, mod - 2, mod);
    };

    for (size_type i = 0; i < sz - 1; i++) {
        const ll v = mul(add(mr[i], mods[i] - cnsts[i], mods[i]), 
                         mod_inv(coffs[i], mods[i]),
                         mods[i]);

        for (size_type j = i + 1; j < sz; j++) {
            cnsts[j] = add(cnsts[j],
                           mul(coffs[j], v, mods[j]),
                           mods[j]);
            coffs[j] = mul(coffs[j],
                           mods[i],
                           mods[j]);
                           
        }
	}

    return cnsts.back();
}

}
