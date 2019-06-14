/*
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
*/

struct TreeCentroid {

    using NodeLabel = ll;
    using Depth = ll;
    using Centroid = pair<NodeLabel, Depth>;
    
    const VV<ll> &edges;
    const ll vertex_num;
    V<bool> used;
    V<Centroid> centroid;
    V<ll> nxt_root;
    ll centroid_level;
    
    TreeCentroid(const VV<ll> &edges) 
        : edges(edges),
          used(edges.size()),
          vertex_num(edges.size()),
          nxt_root(1, 0),
          centroid_level(0)
    {
    }

    ll dfs(ll now, V<bool> &calced, V<ll> &result) {
        ll ret = 0;
        calced[now] = true;
        for(ll nxt : edges[now]) {
            if(used[nxt]) continue;
            if(calced[nxt]) continue;
            ret += dfs(nxt, calced, result);
        }

        return result[now] = ret + 1;
    }

    V<ll> calc_subtree_size() {
        V<bool> calced(vertex_num, false);
        V<ll> subtree_size(vertex_num, -1);
        for(ll root : nxt_root) dfs(root, calced, subtree_size);
        return move(subtree_size);
    }

    ll calc_centroid(ll root, ll pre_node, const V<ll> &subtree_size_v, ll prop_val, ll arg_subtree_size = -1) {
        ll subtree_size = (arg_subtree_size == -1 ? subtree_size_v[root] : arg_subtree_size);
        if(subtree_size == 1) return root;
        V<PLL> child_size;
        ll sum = 0;
        for(ll nxt : edges[root]) {
            if(used[nxt]) continue;
            ll num = (nxt == pre_node ? prop_val : subtree_size_v[nxt]);
            child_size.emplace_back(num, nxt);
            sum += num;
        }
        sort(ALLR(child_size));
        if(child_size[0].first <= subtree_size / 2) return root;
        else return calc_centroid(child_size[0].second, root, subtree_size_v, sum - child_size[0].first + 1, subtree_size);
    }

    void calc_centroid() {
        V<ll> nxt_nxt_root;
        auto subtree_size = calc_subtree_size();
        for(ll node : nxt_root) {
            ll cen = calc_centroid(node, -1, subtree_size, -1);
            centroid.emplace_back(cen, centroid_level);
            used[cen] = true;
            for(ll nxt : edges[cen]) {
                if(used[nxt]) continue;
                nxt_nxt_root.push_back(nxt);
            }
        }
        centroid_level++;
        nxt_root = move(nxt_nxt_root);
        sort(ALL(nxt_root));
        auto ite = unique(ALL(nxt_root));
        nxt_root.erase(ite, nxt_root.end());
    }

    decltype(centroid) calc() {
        while(nxt_root.size()) {
            calc_centroid();
        }
        return centroid;
    }
};

// solution for https://codeforces.com/contest/321/problem/C
int main() {
    ll N;
    cin >> N;
    VV<ll> edges(N);
    for(ll i = 1; i < N; i++) {
        ll a, b;
        cin >> a >> b;
        a--; b--;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }

    TreeCentroid tc(edges);
    auto centroid = tc.calc();
    V<char> ans(N);
    for(const auto &ele : centroid) {
        ll idx, depth;
        tie(idx, depth) = ele;
        ans[idx] = 'A' + depth;
    }
    for(ll i = 0; i < N; i++) cout << ans[i] << " \n"[i + 1 == N];
    return 0;
}
