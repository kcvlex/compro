#pragma once
#include "../util/template.hpp"

namespace graph {

template <std::size_t Size>
class IndependentSet {
    using bs_t = std::bitset<Size>;
    const std::size_t sz;
    std::array<bs_t, Size> mat;
    std::array<int, Size> deg;
    
    template <std::size_t S>
    void chmax(std::bitset<S> &b1, std::bitset<S> b2) {
        if (b1.count() < b2.count()) b1 = b2;
    }

    void set_watched(bs_t &watched, int i) {
        watched.set(i, 1);
        watched |= mat[i];
    }

    bs_t dfs(bs_t sel, bs_t watched) {
        auto fliped = ~watched;
        int v = fliped._Find_first();
        if (sz <= v) return sel;
        for (int i = fliped._Find_next(v); i < sz; i = fliped._Find_next(i)) if (deg[v] < deg[i]) v = i;
        
        if (deg[v] == 2) {
            sel.set(v, 1);
            set_watched(watched, v);
            return dfs(sel, watched);
        }
        
        bs_t ret = sel;
        
        watched.set(v, 1);
        chmax(ret, dfs(sel, watched));

        sel.set(v, 1);
        chmax(ret, sel);
        set_watched(watched, v);
        chmax(ret, dfs(sel, watched));

        return ret;
    }

public:
    IndependentSet(std::size_t sz) : sz(sz) {
        std::fill(ALL(mat), bs_t(0));
        std::fill(ALL(deg), 0);
    }

    void add_edge(int a, int b) {
        mat[a].set(b, 1);
        mat[b].set(a, 1);
        deg[a]++;
        deg[b]++;
    }

    bs_t solve() {
        bs_t sel, watched;
        for (int i = 0; i < sz; i++) if (deg[i] <= 1 && !watched.test(i)) {
            sel.set(i, 1);
            set_watched(watched, i);
        }
        return dfs(sel, watched);
    }
};

}
