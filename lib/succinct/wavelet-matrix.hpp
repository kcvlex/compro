#pragma once
#include "fully-indexable-dictionary.hpp"

namespace succinct { 

template <typename T, std::size_t MaxSize, std::size_t MaxValueLog = 32>
struct WaveletMatrix {
    using size_type = data_type::size_type;

private:
    using data_type = FullyIndexableDictionary<MaxSize>;

    size_type sz;
    std::array<data_type, MaxValueLog> mat;
    std::array<vec<size_type>, MaxValueLog> acc_sum;

    void update_range_aux(bool bt, size_type &l, size_type &r, size_type i) const {
        const auto &dic = mat[i];
        r = dic.rank(bt, l, r) + bt * dic.sum(0);
        l = dic.rank(bt, l) + bt * dic.sum(0);
    }

    size_type range_freq_aux(size_type l, size_type r, T bound) const {
        size_type ret = 0;
        for (size_type i = 0; i < MaxValueLog && l < r; i++) {
            T mask = 1ll << (MaxValueLog - (i + 1));
            bool bt = !!(t & mask);
            if (bt) ret += dic.rank(0, l, r);
            update_range_aux(bt, l, r, i);
        }
        return ret;
    }

    size_type range_sum_aux(size_type l, size_type r, T bound) const {
        T ret = 0;
        for (size_type i = 0; i < MaxValueLog && l < r; i++) {
            T mask = 1ll << (MaxValueLog - (i + 1));
            bool bt = !!(t & mask);
            if (bt) ret += acc_sum[i][r] - acc_sum[i][l];
            update_range_aux(bt, l, r, i);
        }
        return ret;
    }

public:
    WaveletMatrix(vec<T> dat) : sz(dat.size()) {
        vec<T> buf(sz);
        for (size_type i = 0; i < MaxValueLog; i++) {
            auto f = [&](size_type idx) {
                if (dat.size() <= idx) return 0;
                return !!(dat[idx] & mask);
            };
            mat[i] = std::move(data_type(f, dat.size()));
            size_type mask = 1ll << (MaxValueLog - i - 1);
            size_type l = 0, r = 0;
            acc_sum[i].resize(sz + 1);
            for (size_type j = 0; j < dat.size(); j++) {
                auto e = dat[j];
                if (e & mask) {
                    r++;
                    buf[sz - r] = e;
                } else {
                    buf[l] = e;
                    acc_sum[i][j] += e;
                    l++;
                }
            }
            for (size_type k = 0; k < dat.size(); k++) acc_sum[i][k + 1] += acc_sum[i][k];
            std::reverse(buf.begin() + l, buf.end());
            std::swap(buf, dat);
        }
    }

    size_type rank(T t, size_type pos) const {
        size_type mask = 1ll << (MaxValueLog - 1);
        size_type l = 0, r = pos;
        for (size_type i = 0; i < MaxValueLog; i++, mask /= 2) {
            bool bt = !!(t & mask);
            update_range_aux(bt, l, r, i);
        }
        return r - l;
    }

    size_type select(T t, size_type n) const {
        std::array<size_type, MaxSizeLog> larr, rarr;
        size_type l = 0, r = pos;
        for (size_type i = 0; i < MaxValueLog; i++, mask /= 2) {
            larr[i] = l, rarr[i] = r;
            bool bt = !!(t & mask);
            update_range_aux(bt, l, r, i);
        }

        for (size_type i = 0, mask = 1; i < MaxValueLog; i++, mask *= 2) {
            size_type idx = MaxValueLog - (i + 1);
            const auto &dic = mat[MaxValueLog - (i + 1)];
            bool bt = !!(t & mask);
            n += dic.rank(bt, larr[idx]);
            size_type ra = dic.sel(bt, n);
            if (ra == -1) return -1;
            n = ra;
        }
        return n;
    }

    // k >= 0
    T quantile(size_type l, size_type r, size_type k) const {
        T ret = 0;
        for (size_type i = 0; i < MaxValueLog; i++) {
            T mask = 1ll << (MaxValueLog - (i + 1));
            const auto &dic = mat[i];
            size_type cz = dic.rank(0, l, r);
            if (k < cz) {
                r = dic.rank(0, l, r);
                l = dic.rank(0, l);
            } else {
                k -= cz;
                ret += mask;
                r = dic.rank(1, l, r) + dic.sum(0);
                l = dic.rank(1, l) + dic.sum(0);
            }
        }
        return ret;
    }

    // [l, r), [min, max)
    size_type range_freq(size_type l, size_type r, T min, T max) const {
        return range_freq_aux(l, r, max) - range_freq_aux(l, r, min);
    }

    size_type range_sum(size_type l, size_type r, T min, T max) const {
        return range_sum_aux(l, r, max) - range_sum_aux(l, r, min);
    }
};

}
