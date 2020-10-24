#pragma once
#include "suffix-array.hpp"

namespace strings {

template <typename Container>
struct LongestCommonPrefixArray {
    const SuffixArray<Container> &sa;
    vec<size_type> lcp, rank;

    void build() {
        for (size_type i = 0; i < sa.size(); i++) rank[sa[i]] = i;
        size_type h = 0;
        decltype(auto) str = sa.raw();
        for (size_type i = 0; i <= sa.size(); i++) {
            if (rank[i]) {
                size_type j = sa[rank[i] - 1];
                while (true) {
                    if (str.size() <= i + h) break;
                    if (str.size() <= j + h) break;
                    if (str[i + h] != str[j + h]) break;
                    h++;
                }
                lcp[rank[i]] = h;
            }
            h = std::max<size_type>(0, h - 1);
        }
    }

    LongestCommonPrefixArray(const SuffixArray<Container> &sa)
        : sa(sa), lcp(sa.size() + 1), rank(sa.size() + 1)
    {
        build();
    }

    size_type operator [](size_type i) const {
        return lcp[i];
    }
};

}
