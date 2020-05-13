#pragma once
#include "fully-indexable-dictionaly.hpp"

namespace succinct { 

template <typename T, std::size_t MaxSize, std::size_t MaxValueLog = 32>
struct WaveletMatrix {
    using size_type = data_type::size_type;

private:
    using data_type = FullyIndexableDictionaly<MaxSize>;

    size_type sz;
    std::array<data_type, MaxValueLog> mat;
    std::array<size_type, MaxSizeLog> cnt0;

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
            for (auto e : dat) {
                if (e & mask) buf[sz - (++r)] = e;
                else buf[l++] = e;
            }
            cnt0[i] = l;
            std::reverse(buf.begin() + l, buf.end());
            std::swap(buf, dat);
        }
    }

    size_type rank(T t, size_type pos) const {
        size_type mask = 1ll << (MaxValueLog - 1);
        size_type l = 0, r = pos;
        for (size_type i = 0; i < MaxValueLog; i++, mask /= 2) {
            const auto &dic = mat[i];
            bool bt = !!(t & mask);
            size_type nl = dic.rank(bt, l) + bt * cnt0[i];
            size_type nr = dic.rank(bt, l, r) + bt * cnt0[i];
            l = nl, r = nr;
        }
        return r - l;
    }

    size_type select(T t, size_type n) const {
        std::array<size_type, MaxSizeLog> larr, rarr;
        size_type l = 0, r = pos;
        for (size_type i = 0; i < MaxValueLog; i++, mask /= 2) {
            larr[i] = l, rarr[i] = r;
            const auto &dic = mat[i];
            bool bt = !!(t & mask);
            size_type nl = dic.rank(bt, l) + bt * cnt0[i];
            size_type nr = dic.rank(bt, r) - dic.rank(bt, l) + bt * cnt0[i];
            l = nl, r = nr;
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
                size_type nl = dic.rank(0, l);
                size_type nr = dic.rank(0, l, r);
                l = nl, r = nr;
            } else {
                k -= cz;
                ret += mask;
                size_type nl = dic.rank(1, l) + cnt0[i];
                size_type nr = dic.rank(1, l, r) + cnt0[i];
                l = nl, r = nr;
            }
        }
        return ret;
    }
};

}
