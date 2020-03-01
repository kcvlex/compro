// #define DEBUGGING
#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define ALL(V) (V).begin(), (V).end()
#define ALLR(V) (V).rbegin(), (V).rend()

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
template <typename T, typename U> using P = pair<T, U>;
using ll = int64_t;
using ull = uint64_t;
using PLL = P<ll, ll>;

template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return max(t, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }
template <typename T> const T& clamp(const T &t, const T &low, const T &high) { return max(low, min(high, t)); }
template <typename T> void chclamp(T &t, const T &low, const T &high) { t = clamp(t, low, high); }

namespace init__ {

struct InitIO {
    InitIO() {
        cin.tie(nullptr);
        ios_base::sync_with_stdio(false);
        cout << fixed << setprecision(30);
    }
} init_io;

}

#ifdef DEBUGGING
#include "../debug.cpp"
#else
#define DEBUG(...) 0
#define DEBUG_SEPARATOR_LINE 0
#endif

template <typename T>
T make_v(T init) { return init; }

template <typename T, typename... Tail>
auto make_v(T init, size_t s, Tail... tail) {
#define rec make_v(init, tail...)
    return V<decltype(rec)>(s, rec);
#undef rec
}


template <size_t N, size_t Half = 20>
struct IndependentSet {
    using BS = bitset<N>;
    V<BS> edges;
    size_t node_num;
    
    IndependentSet(const VV<ll> &edges_arg) {
        node_num = edges_arg.size();
        edges.resize(node_num);
        for (size_t i = 0; i < node_num; i++) for (size_t j : edges_arg[i]) edges[i].set(j, 1);
    }

    const BS& max_bs(const BS &a, const BS &b) { return a.count() < b.count() ? b : a; }

    // [begin_idx, end_idx)
    V<BS> calc_dp(size_t begin_idx, size_t end_idx) {
        size_t offset = begin_idx;
        size_t node_cnt = end_idx - begin_idx;
        auto make_idx = [&](size_t node_idx) { return node_idx - offset; };
        V<BS> dp(1ll << node_cnt, 0);
        for (size_t S = 0; S < (1ll << node_cnt); S++) {
            BS bs(S);
            bool ok = true;
            for (size_t i = begin_idx; i < end_idx; i++) {
                auto bs_i = bs;
                auto iidx = make_idx(i);
                bs_i.set(iidx, 0);
                ok &= (bs.test(iidx) && (bs_i & (edges[i] >> offset)) != BS(0)) ? false : true;
            }
            dp[S] = (ok ? bs : BS(0));
            for (size_t i = begin_idx; i < end_idx; i++) {
                size_t iidx = make_idx(i);
                size_t pre_set = S ^ (1ll << iidx);
                dp[S] = bs.test(iidx) ? max_bs(dp[S], dp[pre_set]) : dp[S];
            }
        }
        return dp;
    }

    BS calc() {
        size_t first_half = min(node_num, Half);
        auto first_half_dp = calc_dp(0, first_half);

        if (node_num == first_half) return first_half_dp.back();
        
        auto latter_half_dp = calc_dp(first_half, node_num);
        size_t latter_offset = first_half;
        
        BS ret(0);
        BS modify_prefix(0);
        for (size_t i = node_num; i < N; i++) modify_prefix.set(i, 1);
        for (size_t S = 0; S < (1ll << first_half); S++) {
            BS bs(S);
            BS ng_nodes(0);
            for (size_t i = 0; i < first_half; i++) ng_nodes |= (bs.test(i) ? edges[i] : BS(0));
            BS ok_nodes = ~ng_nodes;
            ok_nodes ^= modify_prefix;
            ok_nodes >>= latter_offset;
            BS update = (first_half_dp[S] | (latter_half_dp[ok_nodes.to_ullong()] << latter_offset));
            ret = max_bs(ret, update);
        }

        return ret;
    }
};

// slution for https://code-thanks-festival-2017.contest.atcoder.jp/tasks/code_thanks_festival_2017_g
int main(){
    ll N, M;
    cin >> N >> M;
    vector<vector<ll>> edges(N);
    for (ll i = 0; i < M; i++){
        ll a, b;
        cin >> a >> b;
        a--;
        b--;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }
    auto tmp = IndependentSet<40>(edges).calc();
    cout << tmp.count() << endl;
    return 0;
}
