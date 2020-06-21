#include "../util/template.hpp"
#include "../util/ceil-pow2.hpp"
#include "../util/generics.hpp"
#include "modint.hpp"
#include "base.hpp"

namespace math {

namespace ntt_helper {

class reverse_bit {
    vvec<ll> bits;

    // len = 2^n
    void build_rev_bit(std::size_t len, vec<ll> &v) {
        if (!v.empty()) return;
        uint64_t r = 0, s = 0, max_v = len;
        v.resize(len);
        for (auto &&ele : v) {
            // assert(r < max_v * 2);
            ele = s;
            r += 2;
            s ^= max_v - (max_v / (r & -r));
        }
        // assert(max_v * 2 <= r);
    }

    ll get_idx(std::size_t pow2) {
        ll i;
        for (i = 0; i < 64; i++) if (pow2 & (1ll << i)) break;
        return i;
    }

    void resize(ll idx) {
        if (bits.size() <= idx) bits.resize(idx + 1);
    }

public:
    const vec<ll>& get(std::size_t len) {
        const auto idx = get_idx(len);
        resize(idx);
        build_rev_bit(len, bits[idx]);
        return bits[idx];
    }
};

reverse_bit rb__;

template <ll Mod>
constexpr bool is_primitive_root(ll r) {
    Modint<Mod> mr(r);
    for (ll d = 2; d * d <= Mod; d++) {
        if ((Mod - 1) % d == 0) {
            if (pow(mr, d).value() == 1) return false;
            if (pow(mr, (Mod - 1) / d).value() == 1) return false;
        }
    }
    return true;
}

template <ll Mod>
constexpr ll find_primitive_root(ll r) {
    return (is_primitive_root<Mod>(r) ? r : find_primitive_root<Mod>(r + 1));
}

template <ll Mod>
constexpr ll find_primitive_root() {
    return find_primitive_root<Mod>(2);
}

template <ll Mod, ll Root, std::size_t Size>
struct root_pows_calculator {
    using mint = Modint<Mod>;
    using seq = std::make_integer_sequence<ll, Size>;
    using rseq = decltype(utility::reverse_ns(std::declval<seq>()));

    struct helper {
        mint m;
        ll k;

        constexpr helper(mint m, ll k) : m(m), k(k) { }

        constexpr mint solve() {
            return pow(m, 1ll << k); 
        }
    };

    constexpr static mint root = mint(Root);

    constexpr mint apply(ll k) {
        return helper(root, k).solve(); 
    }

    template <ll... K>
    constexpr auto calc(std::integer_sequence<ll, K...>) -> std::array<mint, Size> {
        return {{ apply(K)... }};
    }

    constexpr auto calc() {
        return calc(rseq());
    }
};

template <ll Mod, ll Root, std::size_t Size>
constexpr auto calc_root_pows() {
    return root_pows_calculator<Mod, Root, Size>().calc();
}

constexpr auto calc_max_base(ll m) {
    ll ret = 0;
    for (; m % 2 == 0; ret++, m /= 2);
    return ret;
}

}

template <ll Mod, ll PrimitiveRoot, std::size_t MaxSizeLog>
class ntt__ {
    static constexpr ssize_t max_size = 1ll << MaxSizeLog;
    static constexpr ssize_t max_conv_size = max_size * 2;

public:
    using mint = Modint<Mod>;
    using poly = std::array<mint, max_conv_size>;

    constexpr ntt__() { }

    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    void convolution(const InputIterator1 a_begin, const InputIterator1 a_end,
                     const InputIterator2 b_begin, const InputIterator2 b_end,
                     OutputIterator out) 
    {
        auto asz = std::distance(a_begin, a_end);
        auto bsz = std::distance(b_begin, b_end);
        auto lower_size = asz + bsz - 1;
        auto conv_size = ceil_pow2(lower_size);
        decltype(auto) rev_bit = ntt_helper::rb__.get(conv_size);
        ntt(a_begin, a_end, false, rev_bit, ntt_a.begin());
        ntt(b_begin, b_end, false, rev_bit);
        for (ll i = 0; i < conv_size; i++) ntt_a[i] *= buf[i];
        ntt(ntt_a.begin(), ntt_a.begin() + conv_size, true, rev_bit, out);
    }

    template <typename InputIterator1, typename InputIterator2>
    const poly& convolution(const InputIterator1 a_begin, const InputIterator1 a_end,
                            const InputIterator2 b_begin, const InputIterator2 b_end)
    {
        convolution(a_begin, a_end, b_begin, b_end, buf.begin());
        return buf;
    }

private:
    using pows_type = std::array<mint, MaxSizeLog>;
    constexpr static ll root_max_pow = pow(mint(PrimitiveRoot), (Mod - 1) / (1ll << MaxSizeLog)).value();
    constexpr static ll root_max_inv = mint(root_max_pow).inv().value();
    constexpr static pows_type root_pow_lis = ntt_helper::calc_root_pows<Mod, root_max_pow, MaxSizeLog>();
    constexpr static pows_type root_inv_lis = ntt_helper::calc_root_pows<Mod, root_max_inv, MaxSizeLog>();
    poly buf, ntt_a;

    template <typename Iterator>
    const poly& ntt(const Iterator begin, const Iterator end, 
                    bool inverse, const vec<ll> &rev_bit) 
    {
        const auto len = rev_bit.size();
        
        {
            ssize_t arr_idx = 0;
            auto sz = std::distance(begin, end);
            for (auto &&idx : rev_bit) {
                buf[idx] = (arr_idx < sz ? *(begin + arr_idx) : 0);
                arr_idx++;
            }
        }

        ssize_t unit_size = 2;
        ssize_t root_pow_idx = 0;
        const auto &root_lis = (inverse ? root_inv_lis : root_pow_lis);
        while (unit_size <= len) {
            mint root = root_lis[root_pow_idx];
            mint root_pow = 1;
            auto unit_cnt = len / unit_size;
            for (ll offset = 0; offset < unit_size / 2; offset++) {
                for (ll unit_counter = 0; unit_counter < unit_cnt; unit_counter++) {
                    auto i = unit_counter * unit_size + offset;
                    auto j = i + unit_size / 2;
                    auto cur_val_i = buf[i], cur_val_j = buf[j];
                    cur_val_j *= root_pow;
                    buf[i] = cur_val_i + cur_val_j;
                    buf[j] = cur_val_i - cur_val_j;
                }
                root_pow *= root;
            }
            unit_size *= 2;
            root_pow_idx++;
        }
        if (inverse) {
            auto inv_len = mint(len).inv();
            for (ssize_t i = 0; i < rev_bit.size(); i++) buf[i] *= inv_len;
        }

        return buf;
    }

    template <typename InputIterator, typename OutputIterator>
    const void ntt(const InputIterator begin, const InputIterator end, 
                   bool inverse, const vec<ll> &rev_bit, OutputIterator out) 
    {
        ntt(begin, end, inverse, rev_bit);
        auto len = rev_bit.size();
        bool copy = true;
        if constexpr (std::is_same<OutputIterator, typename decltype(buf)::iterator>::value) {
            if (out == buf.begin()) copy = false;
        } else {
        }
        if (copy) std::copy(buf.cbegin(), buf.cbegin() + len, out);
    }
};

template <ll Mod>
using NTT = ntt__<Mod, ntt_helper::find_primitive_root<Mod>(), ntt_helper::calc_max_base(Mod - 1)>;

}
