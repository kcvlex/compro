#include <bits/stdc++.h>
using namespace std;
#define ALL(V) (V).begin(), (V).end()

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;

template <uint64_t Mod, uint64_t PrimitiveRoot>
struct NTT {
    class ReverseBit {
        uint64_t r, s, max_v, max_v_twice;

    public:
        ReverseBit(uint64_t max_v__) : r(0), s(0), max_v(max_v__), max_v_twice(max_v__ * 2) {}
        uint64_t get() {
            if(max_v_twice <= r) return numeric_limits<uint64_t>::max();
            auto ret = s;
            r += 2;
            s ^= max_v - (max_v / (r & -r));
            return ret;
        }
    };
    
    V<uint64_t> root_pow_lis, root_inv_lis;
    size_t max_size_log;
    
    NTT(size_t max_size_log) 
        : max_size_log(max_size_log),
          root_pow_lis(max_size_log),
          root_inv_lis(max_size_log)
    {
        auto root_max_pow = this->pow(PrimitiveRoot, (Mod - 1) / (1ll << max_size_log));
        root_pow_lis[0] = root_max_pow;
        for(size_t i = 1; i < root_pow_lis.size(); i++) root_pow_lis[i] = root_pow_lis[i - 1] * root_pow_lis[i - 1] % Mod;
        auto root_max_inv = this->inv(root_max_pow);
        root_inv_lis[0] = root_max_inv;
        for(size_t i = 1; i < root_inv_lis.size(); i++) root_inv_lis[i] = root_inv_lis[i - 1] * root_inv_lis[i - 1] % Mod;
        reverse(ALL(root_pow_lis));
        reverse(ALL(root_inv_lis));
    }

    uint64_t pow(uint64_t n, uint64_t k) { return k == 0 ? 1 : (k & 1 ? n : 1) * this->pow(n * n % Mod, k / 2) % Mod; }

    uint64_t inv(uint64_t n) { return this->pow(n, Mod - 2); }

    V<uint64_t> ntt(const V<uint64_t> &arr, bool inverse, size_t len = 0) {
        if(len == 0) {
            len = 1;
            while(len < arr.size()) len *= 2;
        }

        V<uint64_t> res(len);

        {
            ReverseBit rev_bit(len);
            auto idx = rev_bit.get();
            do {
                res[idx] = (idx < arr.size() ? arr[idx] : 0);
            } while((idx = rev_bit.get()) != numeric_limits<uint64_t>::max());
        }

        size_t unit_size = 2;
        size_t root_pow_idx = 0;
        const auto &root_lis = (inverse ? root_inv_lis : root_pow_lis);
        while(unit_size < res.size()) {
            uint64_t root = root_pow_lis[root_pow_idx];
            uint64_t root_pow = 1;
            auto unit_cnt = res.size() / unit_size;
            for(size_t offset = 0; offset < unit_size / 2; offset++) {
                for(size_t unit_counter = 0; unit_counter < unit_cnt; unit_counter++) {
                    auto i = unit_counter + offset;
                    auto j = i + unit_size / 2;
                    auto cur_val_i = res[i], cur_val_j = res[j];
                    (cur_val_j *= root_pow) %= Mod;
                    res[i] = (cur_val_i + cur_val_j) % Mod;
                    res[j] = (cur_val_i + (Mod - cur_val_j)) % Mod;
                }
                root_pow *= 2;
            }
            unit_size *= 2;
            root_pow_idx++;
        }

        if(inverse) {
            auto inv_len = inv(len);
            for(auto &&ele : res) (ele *= inv_len) %= Mod;
        }

        return res;
    }

    V<uint64_t> convolution(const V<uint64_t> &arr_a, const V<uint64_t> &arr_b) {
        auto lower_size = arr_a.size() + arr_b.size() - 1;
        size_t conv_size = 1;
        while(conv_size < lower_size) conv_size *= 2;
        auto ntt_a = ntt(arr_a, false, conv_size);
        auto ntt_b = ntt(arr_b, false, conv_size);
        for(size_t i = 0; i < conv_size; i++) (ntt_a[i] *= ntt_b[i]) %= Mod;
        return ntt(ntt_a, true, conv_size);
    }
};
