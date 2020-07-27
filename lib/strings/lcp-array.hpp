#pragma once
#include "suffix-array.hpp"

namespace strings {

struct LongestCommonPrefixArray {
    const SuffixArray &sa;
    vec<ll> lcp, rank;

    void build() {
        for (ll i = 0; i < sa.size(); i++) rank[sa[i]] = i;
        ll h = 0;
        const auto &str = sa.str();
        for (ll i = 0; i < sa.size(); i++) {
            if (rank[i] + 1 == sa.size()) continue;
            ll j = sa[rank[i] + 1];
            while (true) {
                if (str.size() <= i + h) break;
                if (str.size() <= j + h) break;
                if (str[i + h] != str[j + h]) break;
                h++;
            }
            lcp[rank[i] + 1] = h;
            h = std::max<ll>(0, h - 1);
        }
    }

    LongestCommonPrefixArray(const SuffixArray &sa)
        : sa(sa), lcp(sa.size()), rank(sa.size())
    {
        build();
    }

    ll operator [](std::size_t i) const {
        return lcp[i];
    }
};

}
