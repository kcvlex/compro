#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
using ll = int64_t;
using PLL = pair<ll, ll>;

template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return max(t, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }
template <typename T> const T& clamp(const T &t, const T &low, const T &high) { return max(low, min(high, t)); }
template <typename T> void chclamp(T &t, const T &low, const T &high) { t = clamp(t, low, high); }

template <size_t N, size_t Half = 20>
struct IndependentSet {
    using BS = bitset<N>;
    V<BS> edges;
    
    IndependentSet(const VV<ll> &edges_arg) : edges(edge2edge(edges_arg)) {}

    V<BS> edge2edge(const VV<ll> &edges) {
        V<BS> ret(N);
        for(ll i = 0; i < N; i++) for(ll j : edges[i]) ret[i].set(j, 1);
        return move(ret);
    }

    const BS& max_bs(const BS &a, const BS &b) { return a.count() < b.count() ? b : a; }

    // [begin_idx, end_idx)
    V<BS> calc_dp(ll begin_idx, ll end_idx) {
        ll offset = begin_idx;
        ll node_cnt = end_idx - begin_idx;
        auto make_idx = [&](ll node_idx) { return node_idx - offset; };
        V<BS> dp(1ll << node_cnt, 0);
        for(ll S = 0; S < (1ll << node_cnt); S++) {
            BS bs(S);
            bool ok = true;
            for(ll i = begin_idx; i < end_idx; i++) {
                for(ll j = i + 1; j < end_idx; j++) {
                    ll iidx = make_idx(i);
                    ll jidx = make_idx(j);
                    ok &= (bs.test(iidx) & bs.test(jidx) & edges[iidx].test(jidx)) ? false : true;
                }
            }
            dp[S] = (ok ? bs : BS(0));
            for(ll i = begin_idx; i < end_idx; i++) {
                ll iidx = make_idx(i);
                ll pre_set = S ^ (1ll << iidx);
                dp[S] = bs.test(iidx) ? max_bs(dp[S], dp[pre_set]) : dp[S];
            }
        }
        return move(dp);
    }

    BS calc() {
        ll first_half = min<ll>(N, Half);
        ll latter_half = N - first_half;
        
        auto first_half_dp = calc_dp(0, first_half);
        
        if(latter_half == 0) return first_half_dp.back();
        
        auto latter_half_dp = calc_dp(first_half, N);
        ll latter_offset = first_half;
        
        BS ret = 0;
        for(ll S = 0; S < (1ll << first_half); S++) {
            BS bs(S);
            BS ng_nodes(0);
            for(ll i = 0; i < first_half; i++) ng_nodes |= (bs.test(i) ? edges[i] : BS(0));
            ng_nodes.flip();
            ng_nodes >>= latter_offset;
            BS update = first_half_dp[S] | (latter_half_dp[ng_nodes.to_ullong()] << latter_offset);
            ret = max_bs(ret, update);
        }

        return move(ret);
    }
};



// slution for https://code-thanks-festival-2017.contest.atcoder.jp/tasks/code_thanks_festival_2017_g
ll N, M;
vector<string> ID;
vector<vector<ll>> fneeds(1e5 + 10);
vector<vector<ll>> edges(40);
bool bad[40][40];

int main(){
    ios::sync_with_stdio(false);
    cin >> N >> M;
    edges.resize(M);
    ll last1 = 0;
    for(ll i = 0; i < N; i++){
        ll n;
        cin >> n;
        if(n == 1){
            last1 = i;
        }else{
            string s;
            cin >> s;
            auto ite = find(ID.begin(), ID.end(), s);
            if(ite == ID.end()){
                ID.push_back(s);
            }
            ite = find(ID.begin(), ID.end(), s);
            ll num = ite - ID.begin();
            fneeds[last1].push_back(num);
        }
    }
    for(auto &v : fneeds){
        if(v.empty()) continue;
        sort(v.begin(), v.end());
        auto ite = unique(v.begin(), v.end());
        v.erase(ite, v.end());
        for(ll i = 0; i < v.size(); i++) for(ll j = 0; j < v.size(); j++) edges[v[i]].push_back(v[j]);
    }
    for(auto &v : edges){
        sort(v.begin(), v.end());
        auto ite = unique(v.begin(), v.end());
        v.erase(ite, v.end());
    }
    cout << IndependentSet<40>(edges).calc().count() << endl;
    return 0;
}
