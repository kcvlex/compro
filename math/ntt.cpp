#include "../util/template.cpp"
#include "modint.cpp"
#include "base.cpp"

namespace math {

namespace {

template <std::size_t MaxSizeLog>
class ReverseBit {
    std::array<vec<ll>, MaxSizeLog> bits;
    std::array<bool, MaxSizeLog> built;

    // len is pow(2, n)
    vec<ll> build_rev_bit(std::size_t len) {
        uint64_t r = 0, s = 0, max_v = len;
        vec<ll> ret(len);
        for (auto &&ele : ret) {
            // assert(r < max_v * 2);
            ele = s;
            r += 2;
            s ^= max_v - (max_v / (r & -r));
        }
        // assert(max_v * 2 <= r);
        return ret;
    }

    ll get_idx(std::size_t pow2) {
        ll i;
        for (i = 0; i < 64; i++) if (pow2 & (1ll << i)) break;
        return i;
    }

public:
    ReverseBit() {
        std::fill(ALL(built), false);
    }

    const vec<ll>& get(std::size_t len) {
        const auto idx = get_idx(len);
        if (built[idx]) return bits[idx];
        bits[idx] = std::move(build_rev_bit(len));
        built[idx] = true;
        return bits[idx];
    }
};

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

}  // anonymous

template <ll Mod, ll PrimitiveRoot, std::size_t MaxSizeLog>
class NTT__ {
    static constexpr std::size_t max_size = 1ll << MaxSizeLog;
    static constexpr std::size_t max_conv_size = max_size * 2;

public:
    using poly = std::array<ll, max_conv_size>;

    NTT__() {
        auto root_max_pow = this->pow(PrimitiveRoot, (Mod - 1) / (1ll << MaxSizeLog));
        root_pow_lis[0] = root_max_pow;
        root_inv_lis[0] = this->inv(root_max_pow);
        for (size_t i = 1; i < root_pow_lis.size(); i++) {
            root_pow_lis[i] = root_pow_lis[i - 1] * root_pow_lis[i - 1] % Mod;
            root_inv_lis[i] = root_inv_lis[i - 1] * root_inv_lis[i - 1] % Mod;
        }
        std::reverse(ALL(root_pow_lis));
        std::reverse(ALL(root_inv_lis));
    }

    template <typename Container1, typename Container2>
    const poly& convolution(const Container1 &arr_a, const Container2 &arr_b) {
        auto lower_size = arr_a.size() + arr_b.size() - 1;
        std::size_t conv_size = 1;
        while (conv_size < lower_size) conv_size *= 2;
        decltype(auto) rev_bit = rev_bits.get(conv_size);
        ntt(arr_a, false, rev_bit, ntt_a);
        ntt(arr_b, false, rev_bit, buf);
        for (std::size_t i = 0; i < conv_size; i++) (ntt_a[i] *= buf[i]) %= Mod;
        return ntt(ntt_a, true, rev_bit);
    }

private:
    std::array<ll, MaxSizeLog> root_pow_lis, root_inv_lis;
    poly buf, ntt_a;
    ReverseBit<MaxSizeLog> rev_bits;

    ll pow(ll n, ll k) {
        Modint<Mod> mn(n);
        return math::pow(mn, k).value();
    }

    ll inv(ll n) { return Modint<Mod>(n).inv().value(); }

    template <typename Container>
    const poly& ntt(const Container &arr, bool inverse, const vec<ll> &rev_bit) {
        const auto len = rev_bit.size();
        
        {
            size_t arr_idx = 0;
            for (auto &&idx : rev_bit) buf[idx] = (arr_idx < arr.size() ? arr[arr_idx++] : 0);
        }

        size_t unit_size = 2;
        size_t root_pow_idx = 0;
        const auto &root_lis = (inverse ? root_inv_lis : root_pow_lis);
        while (unit_size <= len) {
            uint64_t root = root_lis[root_pow_idx];
            uint64_t root_pow = 1;
            auto unit_cnt = len / unit_size;
            for (size_t offset = 0; offset < unit_size / 2; offset++) {
                for (size_t unit_counter = 0; unit_counter < unit_cnt; unit_counter++) {
                    auto i = unit_counter * unit_size + offset;
                    auto j = i + unit_size / 2;
                    auto cur_val_i = buf[i], cur_val_j = buf[j];
                    (cur_val_j *= root_pow) %= Mod;
                    buf[i] = (cur_val_i + cur_val_j) % Mod;
                    buf[j] = (cur_val_i + (Mod - cur_val_j)) % Mod;
                }
                (root_pow *= root) %= Mod;
            }
            unit_size *= 2;
            root_pow_idx++;
        }
        if (inverse) {
            auto inv_len = inv(len);
            for (auto &&ele : buf) (ele *= inv_len) %= Mod;
        }

        return buf;
    }

    template <typename Container1, typename Container2>
    const void ntt(const Container1 &arr, bool inverse, const vec<ll> &rev_bit, Container2 &out) {
        ntt(arr, inverse, rev_bit);
        auto len = rev_bit.size();
        std::copy(buf.cbegin(), buf.cbegin() + len, out.begin());
    }
};

template <ll Mod, std::size_t MaxSizeLog>
using NTT = NTT__<Mod, find_primitive_root<Mod>(), MaxSizeLog>;

}
