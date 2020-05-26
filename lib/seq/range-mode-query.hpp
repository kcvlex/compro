#pragma once
#include "../util/template.hpp"
#include "../util/compress.hpp"
#include "../util/ceil-div.hpp"

namespace seq {

struct RangeModeQuery {
    using size_type = ssize_t;

    // v must be compressed.
    RangeModeQuery(const vec<size_type> &v, const size_type kinds)
        : v(v),
          blk_sz(ceil_sqrt(kinds)), 
          blk_cnt(utility::ceil_div(kinds, blk_sz)),
          idx_v(kinds), 
          blk_mode(make_v<size_type>(blk_cnt, blk_cnt + 1)),
          blk_freq(make_v<size_type>(blk_cnt, blk_cnt + 1)),
          rank(kinds, 0), buf(kinds, 0)
    {
        for (size_type i = 0; i < blk_sz; i++) {
            size_type max_idx = 0;

            for (size_type j = i + 1; j <= blk_sz; j++) {
                size_type begin = (j - 1) * blk_sz, end = j * blk_sz;
                for (size_type k = begin; k < end; k++) {
                    auto e = v[k];
                    buf[e]++;
                    if (buf[max_idx] < buf[e]) max_idx = e;
                }

                blk_mode[i][j] = max_idx;
                blk_freq[i][j] = buf[max_idx];
                std::fill(ALL(buf), 0);
            }
        }
        
        for (size_type i = 0; i < v.size(); i++) {
            auto e = v[i];
            idx_v[e].push_back(i);
            rank[i] = buf[e]++;
        }
        std::fill(ALL(buf), 0);
    }

    // element, freq
    // [l, r)
    auto query(size_type l, size_type r) -> std::pair<size_type, size_type> {
        size_type lblk = l / blk_sz + !!(l % blk_sz);
        size_type rblk = r / blk_sz - !!(r % blk_sz);

        size_type mode = blk_mode[lblk][rblk];
        size_type freq = blk_freq[lblk][rblk];
        buf[mode] = freq;

        // prefix
        for (size_type i = l; i < lblk * blk_sz; i++) {
            auto e = v[i];
            if (buf[e]) continue;
            
            auto rk = rank[i];
            auto idx = rk + freq;
            if (idx_v[e].size() <= idx || r < idx_v[e][idx]) {
                buf[e] = -1;
                continue;
            }
            
            while (idx < idx_v[e].size() && idx_v[e][idx] <= r) idx++;
            auto sz = idx - rk;
            buf[e] = sz;
            if (freq < sz) {
                mode = e;
                freq = sz;
            }
        }

        // suffix
        for (size_type i = rblk * blk_sz; i < r; i++) {
            auto e = v[i];
            if (buf[e]) continue;

            auto rk = rank[i];
            auto idx = rk - freq;
            if (idx < 0 || idx_v[e][idx] < l) {
                buf[e] = -1;
                continue;
            }

            while (0 <= idx && l <= idx_v[e][idx]) idx--;
            auto sz = rk - idx;
            buf[e] = sz;
            if (freq < sz) {
                mode = e;
                freq = sz;
            }
        }

        for (size_type i = l; i < lblk * blk_sz; i++) buf[i] = 0;
        for (size_type i = rblk * blk_sz; i < r; i++) buf[i] = 0;
        buf[blk_mode[lblk][rblk]] = 0;

        return std::make_pair(mode, freq);
    }

private:
    const vec<size_type> &v;
    const size_type blk_sz, blk_cnt;
    vvec<size_type> idx_v, blk_mode, blk_freq;
    vec<size_type> rank, buf;

    size_type ceil_sqrt(size_type s) const {
        size_type ret = 1;
        while (ret * ret < s) ret++;
        return ret;
    }
};

}
