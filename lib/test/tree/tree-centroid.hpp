#include "../tree/tree-centroid.cpp"

// solution for https://codeforces.com/contest/321/problem/C

void dfs(ll cur, vec<char> &ans, vvec<ll> &cen, ll depth = 0) {
    ans[cur] = 'A' + depth;
    for (auto &&nxt : cen[cur]) dfs(nxt, ans, g, depth + 1);
}

int main() {
    ll n;
    std::cin >> n;
    graph::Graph<false> g(n);
    for (ll i = 0; i < n - 1; i++) {
        ll x, y;
        std::cin >> x >> y;
        g.add_edge(x - 1, y - 1);
    }

    ll root;
    vvec<ll> cens;
    std::tie(root, cens) = std::move(tree::tree_centroid(g));
    vec<char> ans(n);
    dfs(root, ans, cens);
    for (ll i = 0; i < n; i++) std::cout << ans[i] << " \n"[i + 1 == n];
    return 0;
}
