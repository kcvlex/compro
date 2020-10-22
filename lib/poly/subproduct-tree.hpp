#pragma once
#include "fps.hpp"

namespace poly {

template <typename Poly>
struct SubproductTree {
    using value_type = typename Poly::value_type;

    template <typename Container>
    SubproductTree(const Container &cs) {
        rebuild(cs);
    }

    size_type size() const noexcept {
        return sz;
    }

    size_type height() const {
        return rank.back();
    }

    size_type get_len(size_type r) const {
        return len_sum[r + 1] - len_sum[r];
    }

    // rank, index
    size_type get_poly_idx(size_type r, size_type idx) const {
        return idx + len_sum[r];
    }

    const Poly& get_poly(size_type r, size_type idx) const {
        return nodes[get_poly_idx(r, idx)];
    }

    value_type operator[](size_type i) const {
        return cs[i];
    }

private:
    vec<Poly> nodes;
    vec<value_type> cs;
    vec<ll> rank;
    vec<size_type> len_sum;
    size_type sz, nsz;

    template <typename Container>
    void rebuild(const Container &cs) {
        resize(cs.size());
        std::copy(ALL(cs), this->cs.begin());

        size_type idx = 0;
        for (; idx < sz; idx++) {
            nodes[idx] = Poly { value_type(-cs[idx]), value_type(1) };
            rank[idx] = 0;
        }

        size_type len = sz;
        while (1 < len) {
            const size_type offset = idx - len;
            const size_type r = rank[idx - 1] + 1;
            for (size_type i = 0; i + 1 < len; i += 2, idx++) {
                nodes[idx] = std::move(mul(nodes[i + offset], nodes[i + 1 + offset]));
                rank[idx] = r;
            }
            if (len & 1) {
                nodes[idx] = nodes[offset + len - 1];
                rank[idx] = r;
                idx++;
            }
            len = (len + 1) / 2;
        }

        len_sum.resize(height() + 2);
        for (size_type i = 0; i < nsz; i++) len_sum[rank[i] + 1]++;
        for (size_type i = 0; i + 1 < height() + 2; i++) len_sum[i + 1] += len_sum[i];
    }

    void resize(size_type sz_arg) {
        sz = sz_arg;
        nsz = sz_arg;
        while (1 < sz_arg) {
            auto nxt = (sz_arg + 1) / 2;
            sz_arg = nxt;
            nsz += nxt;
        }
        nodes.resize(nsz);
        rank.resize(nsz);
        cs.resize(nsz);
    }
};

}
