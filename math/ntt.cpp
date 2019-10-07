#include <bits/stdc++.h>
using namespace std;
#define ALL(V) (V).begin(), (V).end()

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;

/*
 * https://lumakernel.github.io/ecasdqina/math/FFT/NTT
constexpr ll NTT_PRIMES[][2] = {
    {1224736769, 3}, // 2^24 * 73 + 1,
    {1053818881, 7}, // 2^20 * 3 * 5 * 67 + 1
    {1051721729, 6}, // 2^20 * 17 * 59 + 1
    {1045430273, 3}, // 2^20 * 997 + 1
    {1012924417, 5}, // 2^21 * 3 * 7 * 23 + 1
    {1007681537, 3}, // 2^20 * 31^2 + 1
    {1004535809, 3}, // 2^21 * 479 + 1
    {998244353, 3},  // 2^23 * 7 * 17 + 1
    {985661441, 3},  // 2^22 * 5 * 47 + 1
    {976224257, 3},  // 2^20 * 7^2 * 19 + 1
    {975175681, 17}, // 2^21 * 3 * 5 * 31 + 1
    {962592769, 7},  // 2^21 * 3^3 * 17 + 1
    {950009857, 7},  // 2^21 * 4 * 151 + 1
    {943718401, 7},  // 2^22 * 3^2 * 5^2 + 1
    {935329793, 3},  // 2^22 * 223 + 1
    {924844033, 5},  // 2^21 * 3^2 * 7^2 + 1
    {469762049, 3},  // 2^26 * 7 + 1
    {167772161, 3},  // 2^25 * 5 + 1
};
*/

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
        root_inv_lis[0] = this->inv(root_max_pow);
        for(size_t i = 1; i < root_pow_lis.size(); i++) {
            root_pow_lis[i] = root_pow_lis[i - 1] * root_pow_lis[i - 1] % Mod;
            root_inv_lis[i] = root_inv_lis[i - 1] * root_inv_lis[i - 1] % Mod;
        }
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
            auto res_idx = rev_bit.get();
            size_t arr_idx = 0;
            do {
                res[res_idx] = (arr_idx < arr.size() ? arr[arr_idx++] : 0);
            } while((res_idx = rev_bit.get()) != numeric_limits<uint64_t>::max());
        }

        size_t unit_size = 2;
        size_t root_pow_idx = 0;
        const auto &root_lis = (inverse ? root_inv_lis : root_pow_lis);
        while(unit_size <= res.size()) {
            uint64_t root = root_lis[root_pow_idx];
            uint64_t root_pow = 1;
            auto unit_cnt = res.size() / unit_size;
            for(size_t offset = 0; offset < unit_size / 2; offset++) {
                for(size_t unit_counter = 0; unit_counter < unit_cnt; unit_counter++) {
                    auto i = unit_counter * unit_size + offset;
                    auto j = i + unit_size / 2;
                    auto cur_val_i = res[i], cur_val_j = res[j];
                    (cur_val_j *= root_pow) %= Mod;
                    res[i] = (cur_val_i + cur_val_j) % Mod;
                    res[j] = (cur_val_i + (Mod - cur_val_j)) % Mod;
                }
                (root_pow *= root) %= Mod;
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

// https://atcoder.jp/contests/atc001/tasks/fft_c
int main() {
    NTT<1224736769, 3> ntt(20);
    int N;
    cin >> N;
#if 1
    V<uint64_t> A(N + 1), B(N + 1);
    for(int i = 0; i < N; i++) cin >> A[i + 1] >> B[i + 1];
    auto conv = ntt.convolution(A, B);
    for(int i = 1; i <= 2 * N; i++) cout << conv[i] << endl;
#else 
    V<uint64_t> A(N), B(N);
    for(int i = 0; i < N; i++) cin >> A[i] >> B[i];
    auto conv = ntt.convolution(A, B);
    cout << 0 << endl;
    for(int i = 0; i < 2 * N - 1; i++) cout << conv[i] << endl;
#endif
    return 0;
}
