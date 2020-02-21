#include "suffixarray.cpp"

namespace str {

class LongestCommonPrefixArray {
    const SuffixArray &sa;
    V<ll> lcp, rank;

    void build() {
        for (ll i = 0; i < sa.size(); i++) rank[sa[i]] = i;
        ll h = 0;
        const auto &str = sa.str();
        for (ll i = 0; i < sa.size(); i++) {
            if (rank[i] + 1 == sa.size()) continue;
            ll cur = i, nxt = sa[rank[cur] + 1];
            while (true) {
                if (cur + h < sa.size()) break;
                if (nxt + h < sa.size()) break;
                if (str[cur + h] != str[nxt + h]) break;
                h++;
            }
            lcp[rank[i] + 1] = h;
            h = max<ll>(0, h - 1);
        }
    }

public:
    LongestCommonPrefixArray(const SuffixArray &sa)
        : sa(sa), lcp(sa.size()), rank(sa.size())
    {
        build();
    }
};

}
