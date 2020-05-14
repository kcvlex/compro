#pragma once
#include "../util/template.hpp"
#include "../util/bit-op.hpp"
#include "../util/ceil-div.hpp"

namespace succinct { 

template <std::size_t MaxSize>
struct FullyIndexableDictionaly {
    using size_type = ssize_t;

private:
    constexpr static size_type sz_log2 = utility::ceil_log2(max_size);
    constexpr static size_type chunk_sz = sz_log2 * sz_log2;
    constexpr static size_type block_sz = sz_log2 / 2;
    constexpr static size_type block_per_chunk = chunk_sz / block_sz;

    size_type sz, cnt1;
    vec<size_type> dat;
    vec<size_type> chunk;
    vvec<size_type> block;

public:
    FullyIndexableDictionaly() { } 

    template <typename F>
    FullyIndexableDictionaly(F f, size_type sz) 
        : sz(sz), cnt1(0),
          dat(ceil_div(sz, block_sz), 0), 
          chunk(ceil_div(sz, chunk_sz)), 
          block(make_v<block_type>(chunk.size(), block_per_chunk))
    {
        for (size_type i = 0; i < chunk.size(); i++) {
            chunk[i] = cnt1;
            size_type tmp = 0;
            for (size_type j = 0; j < block[i].size(); j++) {
                block[i][j] = tmp;
                for (size_type k = 0; k < block_sz; k++) {
                    size_type idx = i * chunk_sz + j * block_sz;
                    auto v = f(idx + k);
                    size_type mask = (1ll << k) * v;
                    dat[i * block_per_chunk + j] |= mask;
                    tmp += v;
                }
                block[i][j] = tmp;
            }
            cnt1 += tmp;
        }
    }

    size_type rank(bool b, size_type pos) const {
        if (!b) return pos - rank(!b, pos);
        size_type ret = 0, idx = 0;
        
        size_type i = pos / chunk_sz;
        pos %= chunk_sz;
        idx += i * chunk_sz * block_per_chunk;
        ret += chunk[i];
        
        size_type j = pos / block_sz;
        pos %= block_sz;
        idx += j * block_sz;
        ret += block[i][j];
        
        auto mask = (1ll << rest) - 1;
        return ret + utility::popcount(dat[idx] & mask);
    }

    size_type rank(bool b, size_type l, size_typr r) const {
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
