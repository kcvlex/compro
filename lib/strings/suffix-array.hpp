#pragma once
#include "util/template.hpp"

namespace strings {

template <typename Container,
          size_type Min = 0,
          size_type Max = static_cast<size_type>(std::numeric_limits<char>::max()) + 1>
class SuffixArray {
    const Container &s;
    vec<size_type> ord2idx;

    vec<size_type> build() {
        const size_type sz = s.size() + 1;
        const size_type range = Max - Min;
        vec<size_type> idx2class(sz), ord2idx(sz), cnts(std::max(range, sz) + 1);
        const size_type csz = cnts.size();

        auto get = [&](size_type i) -> size_type {
            if (i < sz) return s[i];
            return Min;  // i == sz;
        };

        std::iota(ALL(ord2idx), 0ll);
        std::sort(ALL(ord2idx), [&](const auto i, const auto j) {
            auto c1 = get(i), c2 = get(j);
            return c1 < c2;
        });

        std::iota(ALL(cnts), 0ll);
        idx2class[ord2idx[0]] = 0;
        for (size_type i = 1; i < sz; i++) {
            size_type i1 = ord2idx[i - 1],
                      i2 = ord2idx[i];
            if (get(i1) != get(i2)) {
                idx2class[i2] = i;
            } else {
                idx2class[i2] = idx2class[i1];
            }
        }

        vec<size_type> buf(sz);
        auto build_step = [&](size_type k) {
            size_type len = 1ll << k;

            std::swap(buf, idx2class);

            // build classes
            idx2class[ord2idx[0]] = 0;
            for (size_type i = 1; i < sz; i++) {
                size_type i1 = ord2idx[i], i2 = ord2idx[i - 1];
                bool same = [&] {
                    if (sz <= i1 + len) return false;
                    if (sz <= i2 + len) return false;
                    if (buf[i1] != buf[i2]) return false;
                    if (buf[i1 + len / 2] != buf[i2 + len / 2]) return false;
                    return true;
                }();
                if (same) {
                    idx2class[i1] = idx2class[i2];
                } else {
                    idx2class[i1] = i;
                }
            }

            // sort
            std::iota(ALL(cnts), 0ll);
            buf = ord2idx;
            for (size_type i = 0; i < sz; i++) {
                auto idx = buf[i];
                if (len <= idx) ord2idx[cnts[idx2class[idx - len]]++] = idx - len;
            }
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
        ll ng = -1, ok = size() - 1;
        while (std::abs(ok - ng) > 1) {
            auto mid = (ok + ng) / 2;
            auto res = comp(oth, ord2idx[mid], 0);
            (check_lower_bound(res) ? ok : ng) = mid;
        }
        return ok;
    }

    size_type upper_bound_aux(const Container &oth) const noexcept {
        ll ng = -1, ok = size() - 1;
        while (abs(ok - ng) > 1) {
            auto mid = (ok + ng) / 2;
            auto res = comp(oth, ord2idx[mid], 0);
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
        auto res = comp(oth, ord2idx.back(), 0);
        if (!check_lower_bound(res)) return size();
        return lower_bound_aux(oth);
    }

    size_type upper_bound(const std::string &oth) const noexcept {
        auto res = comp(oth, ord2idx.back(), 0);
        if (!check_upper_bound(res)) return size();
        return upper_bound_aux(oth);
    }

    const Container& raw() const noexcept {
        return s;
    }
};

}
