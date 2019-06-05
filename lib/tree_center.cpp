#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define ALL(V) (V).begin(), (V).end()
#define ALLR(V) (V).rbegin(), (V).rend()

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
template <typename T, typename U> using P = pair<T, U>;
using ll = int64_t;
using PLL = P<ll, ll>;

template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return max(t, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }
template <typename T> const T& clamp(const T &t, const T &low, const T &high) { return max(low, min(high, t)); }
template <typename T> void chclamp(T &t, const T &low, const T &high) { t = clamp(t, low, high); }

template <typename T>
T make_v(T init) { return init; }

template <typename T, typename... Tail>
auto make_v(T init, size_t s, Tail... tail) {
#define rec make_v(init, tail...)
    return V<decltype(rec)>(s, rec);
#undef rec
}

// TODO : calc_centroid(const VV<ll> &edge, V<bool> &used_for_centroid, ll iter = -1);

PLL calc_centroid_of_tree(const VV<ll> &edges) {
    ll node_cnt = edges.size();
    V<ll> cnt_children(edges.size());

    function<ll(ll, ll)> calc_dp = [&](ll now, ll pre) {
        ll ret = 0;
        for(ll nxt : edges[now]) {
            if(pre == nxt) continue;
            ret += calc_dp(nxt, now);
        }
        return cnt_children[now] = ret + 1;
    };

    calc_dp(0, -1);

    PLL ret(-1, -1);
    ll cnt_c = 0;

    function<void(ll, ll, ll)> calc_centroid = [&](ll now, ll pre, ll max_child_num) {
        V<ll> children_cnt_lis = { 
            { max_children_cnt, pre },
            { -1, -1 }
        };

        for(ll nxt : edges[now]) {
            if(nxt == pre) continue;
            children_cnt_lis.emplace_back(cnt_children[nxt], nxt);
        }

        sort(ALLR(children_cnt_lis));
        if(children_cnt_lis[0] <= node_cnt / 2) {
            (cnt_c == 0 ? ret.first : ret.second) = now;
            cnt_c++;
        }
        
        ll max_cc, max_cc_idx;
        tie(max_cc, max_cc_idx) = chlidren_cnt_lis[0];
        chldren_cnt_lis.erase(chldren_cnt_lis.begin(), children_cnt_lis.end());
        for(ll nxt : edges[now]) {
            if(nxt == pre) continue;
            ll prop_max_cc = (max_cc_idx != nxt ? max_cc + 1
                                                : max(max_cc - 1, children_cnt_lis[1].first));
            calc_centroid(nxt, now, prop_max_cc);
        }
    };

    calc_centroid(0, -1, -1);

    return ret;
}

int main() {
}
