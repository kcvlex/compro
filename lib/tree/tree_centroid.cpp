#include "../template.cpp"

struct TreeCentroid {
    const VV<ll> &edges;
    const ll N;
    V<bool> used;
    V<ll> sub_sz;
    VV<ll> cen_ch;
    ll root;

    TreeCentroid(const VV<ll> &edges) 
        : edges(edges), N(edges.size()), used(edges.size()),
          sub_sz(edges.size()), cen_ch(edges.size()), root(-1)
    { }

    ll dfs(ll cur, ll pre) {
        sub_sz[cur] = 1;
        for(ll nxt : edges[cur]) {
            if(used[nxt]) continue;
            if(nxt == pre) continue;
            sub_sz[cur] += dfs(nxt, cur);
        }
        return sub_sz[cur];
    }

    ll search(ll cur, ll pre, ll tree_size) {
        if(pre == -1) {
            dfs(cur, -1);
            tree_size = sub_sz[cur];
        }
        ll large_ch = -1;
        for(ll ch : edges[cur]) {
            if(used[ch]) continue;
            if(ch == pre) continue;
            if(tree_size / 2 < sub_sz[ch]) {
                large_ch = ch;
                break;
            }
        }

        if(large_ch == -1) {
            used[cur] = true;
            if(root == -1) root = cur;
            for(ll nxt : edges[cur]) {
                if(used[nxt]) continue;
                cen_ch[cur].push_back(search(nxt, -1, -1));
            }
            return cur;
        } else {
            return search(large_ch, cur, tree_size);
        }
    }

    void build() { search(0, -1, N); }
};

// solution for https://codeforces.com/contest/321/problem/C

void dfs(ll cur, V<char> &ans, const VV<ll> &nxts, ll depth = 0) {
    ans[cur] = 'A' + depth;
    for(ll nxt : nxts[cur]) dfs(nxt, ans, nxts, depth + 1);
}

int main() {
    ll N;
    cin >> N;
    VV<ll> edges(N);
    for (ll i = 1; i < N; i++) {
        ll a, b;
        cin >> a >> b;
        a--; b--;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }

    TreeCentroid tc(edges);
    tc.build();
    V<char> ans(N);
    dfs(tc.root, ans, tc.cen_ch);
    for (ll i = 0; i < N; i++) cout << ans[i] << " \n"[i + 1 == N];
    return 0;
}
