#pragma once
#include "../util/template.hpp"
#include "../util/bit-op.hpp"
#include "../util/ceil-div.hpp"

namespace succinct { 

template <typename T>
constexpr T ceil_log2(T n) {
    for (int i = 0; i < 63; i++) {
        T mask = 1ll << i;
        if (n <= mask) return i;
    }
    return -1;
}

template <typename T>
constexpr T ceil_even(T n) {
    return n + (n & 1);
}

template <std::size_t MaxSize>
struct FullyIndexableDictionary {
    using size_type = ssize_t;

private:
    constexpr static size_type sz_log2 = ceil_even(ceil_log2<size_type>(MaxSize));
    constexpr static size_type chunk_sz = sz_log2 * sz_log2;
    constexpr static size_type block_sz = sz_log2 / 2;
    constexpr static size_type block_per_chunk = chunk_sz / block_sz;

    size_type sz, cnt1;
    vec<size_type> chunk, block, dat;

public:
    FullyIndexableDictionary() { } 

    template <typename F>
    FullyIndexableDictionary(F f, size_type sz) 
        : sz(sz), cnt1(0),
          chunk(utility::ceil_div(sz, chunk_sz)), 
          block(chunk.size() * block_per_chunk),
          dat(block.size())
    {
        size_type idx = 0;
        for (size_type i = 0; i < chunk.size(); i++) {
            chunk[i] = cnt1;
            size_type tmp = 0;
            for (size_type j = 0; j < block_per_chunk; j++, idx++) {
                block[idx] = tmp;
                for (size_type k = 0; k < block_sz; k++) {
                    auto v = f(idx * block_sz + k);
                    size_type mask = (1ll << k) * v;
                    dat[idx] |= mask;
                    tmp += v;
                }
            }
            cnt1 += tmp;
        }
    }

    size_type rank(bool b, size_type pos) const {
        if (!b) return pos - rank(!b, pos);
        if (pos == sz) return sum(b);
        size_type ret = 0;
        
        ret += chunk[pos / chunk_sz];
        ret += block[pos / block_sz];

        auto mask = (1ll << (pos % block_sz)) - 1;
        return ret + utility::popcount(dat[pos / block_sz] & mask);
    }

    size_type rank(bool b, size_type l, size_type r) const {
        return rank(b, r) - rank(b, l);
    }

    size_type select(bool b, size_type n) const {
        if (rank(b, sz) < n) return -1;

        size_type ok = sz, ng = 0;
        while (1 < std::abs(ok - ng)) {
            size_type mid = (ok + ng) / 2;
            (n <= rank(b, mid) ? ok : ng) = mid;
        }
        return ok;
    }

    bool operator [](size_type i) const {
        size_type offset = i % block_sz;
        return (dat[i / block_sz] >> offset) & 1;
    }

    size_type sum(bool b) const {
        return b ? cnt1 : sz - cnt1;
    }
};

}
