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

    size_type sz;
    vec<size_type> dat;
    vec<size_type> chunk;
    vvec<size_type> block;

    size_type dat_idx(size_type i, size_type j) const {
        return i * block_per_chunk + j;
    }

public:
    
    FullyIndexableDictionaly() { } 

    template <typename F>
    FullyIndexableDictionaly(F f, size_type sz) 
        : sz(sz), 
          dat(ceil_div(sz, block_sz), 0), 
          chunk(ceil_div(sz, chunk_sz)), 
          block(make_v<block_type>(chunk.size(), block_per_chunk))
    {
        size_tupe sum = 0;
        for (size_type i = 0; i < chunk.size(); i++) {
            chunk[i] = sum;
            size_type tmp = 0;
            for (size_type j = 0; j < block[i].size(); j++) {
                block[i][j] = tmp;
                for (size_type k = 0; k < block_sz; k++) {
                    size_type idx = i * chunk_sz + j * block_sz;
                    auto v = f(idx + k);
                    size_type mask = (1ll << k) * v;
                    dat[dat_idx(i, j)] |= mask;
                    tmp += v;
                }
                block[i][j] = tmp;
            }
            sum += tmp;
        }
    }

    size_type rank(bool b, size_type pos) const {
        if (!b) return pos - rank(!b, pos);
        size_type ret = 0, store = pos;
        
        size_type i = pos / chunk_sz;
        pos %= chunk_sz;
        ret += chunk[i];
        
        size_type j = pos / block_sz;
        pos %= block_sz;
        ret += block[i][j];
        
        size_type rest = store - pos;
        auto mask = (1ll << rest) - 1;
        return ret + utility::popcount(dat[dat_idx(i, j)] & mask);
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
};

}
