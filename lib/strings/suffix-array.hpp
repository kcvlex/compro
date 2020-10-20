#pragma once
#include "../util/template.hpp"

namespace strings {

template <typename Container,
          size_type Min = 0,
          size_type Max = std::numeric_limits<char>::max() + 1>
class SuffixArray {
    const Container &s;
    vec<size_type> ord2idx;
    const size_type range = Max - Min;

    vec<size_type> build() {
        const size_type sz = s.size() + 1;
        vec<size_type> idx2class(sz), ord2idx(sz), cnts(std::max(range, sz) + 1);
        const size_type csz = cnts.size();

        auto get = [&](size_type i) {
            if (i < sz) return s[i];
            return Min;  // i == sz;
        };
        
        for (size_type i = 0; i < sz; i++) cnts[get(i)]++;
        for (size_type i = 0; i + 1 < csz; i++) cnts[i + 1] += cnts[i];
        for (size_type i = sz - 1; 0 <= i; i--) ord2idx[--cnts[get(i)]] = i;

        idx2class[ord2idx[0]] = 0;
        for (size_type i = 1; i < sz; i++) {
            size_type i1 = ord2idx[i - 1],
                      i2 = ord2idx[i];
            idx2class[i2] = idx2class[i1];

            size_type v1 = get(ord2idx[i - 1]),
                      v2 = get(ord2idx[i]);
            if (v1 != v2) idx2class[i2]++;
        }
            
        auto make = [&](size_type i) {
            const size_type a = idx2class[i];
            size_type ni = i + (1ll << k);
            if (sz <= ni) ni -= sz;
            const size_type b = idx2class[ni];
            return std::make_pair(a, b);
        };

        vec<size_type> next_idx2class(sz), next_ord2idx(sz);
        auto build_step = [&](size_type k) {
            for (size_type i = 0; i < sz; i++) {
                auto &v = next_ord2idx[i];
                v = ord2idx[i] - (1ll << i);
                if (v < 0) v += sz;
            }

            std::fill(ALL(cnts), 0);
            for (size_type i = 0; i < sz; i++) cnts[idx2class[i]]++;
            for (size_type i = 0; i + 1 < csz; i++) cnts[i + 1] += cnts[i];
            for (size_type i = sz - 1; 0 <= i; i--) {
                const size_type idx = next_ord2idx[i];
                const size_type cls = idx2class[idx];
                ord2idx[--cnts[cls]] = idx;
            }

            next_idx2class[ord2idx[0]] = 0;
            for (size_type i = 1; i < sz; i++) {
                size_type i1 = ord2idx[i - 1],
                          i2 = ord2idx[i];
                auto cur = make(i1), pre = make(i2);
                next_idx2class[i2] = next_idx2class[i1] + (cur != pre);
            }
            std::swap(next_idx2class, idx2class);
        };

        for (size_type k = 0; (1ll << k) < sz; k++) build_step(k);

        ord2idx.erase(ord2idx.begin());
        return ord2idx;
    }

    bool check_lower_bound(int res) const noexcept {
        return 0 <= res;
    }

    bool check_upper_bound(int res) const noexcept {
        return res == 1;
    }

    size_type lower_bound_aux(const Container &oth) const noexcept {
        size_type ng = -1, ok = size() - 1;
        while (std::abs(ok - ng) > 1) {
            auto mid = (ok + ng) / 2;
            auto res = comp(oth, sa[mid], 0);
            (check_lower_bound(res) ? ok : ng) = mid;
        }
        return ok;
    }

    size_type upper_bound_aux(const Container &oth) const noexcept {
        size_type ng = -1, ok = size() - 1;
        while (abs(ok - ng) > 1) {
            auto mid = (ok + ng) / 2;
            auto res = comp(oth, sa[mid], 0);
            (check_upper_bound(res) ? ok : ng) = mid;
        }
        return ok;
    }

public:
    SuffixArray(const Container &s) : s(s), ord2idx(std::move(build())) { }

    size_type operator[](size_type i) const noexcept {
        return ord2idx[i];
    }

    size_type size() const noexcept {
        return ord2idx.size();
    }

    // compare s[sl:] to oth[tl:]
    // -1 : s < oth
    //  0 : s == oth
    //  1 : s > oth
    int comp(const Container &oth, size_type sl, size_type tl) const noexcept {
        const size_type oth_sz = oth.size();
        for (; sl < size() && tl < oth_sz; sl++, tl++) {
            auto v1 = s[sl];
            auto v2 = oth[tl];
            if (v1 != v2) return (v1 < v2 ? -1 : 1);
        }

        if (sl < size()) return 1;
        if (tl < oth_sz) return -1;
        return 0;
    }

    size_type lower_bound(const std::string &oth) const noexcept {
        auto res = comp(oth, sa.back(), 0);
        if (!check_lower_bound(res)) return size();
        return lower_bound_aux(oth);
    }

    size_type upper_bound(const std::string &oth) const noexcept {
        auto res = comp(oth, sa.back(), 0);
        if (!check_upper_bound(res)) return size();
        return upper_bound_aux(oth);
    }
};

}
