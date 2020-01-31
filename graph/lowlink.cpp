#include "../util/template.cpp"

struct LowLink {
    ll N;
    const VV<ll> &edges;
    V<ll> low, ord;
    V<bool> visited;
    V<bool> is_articulation;
    ll dfs_counter;

    LowLink(const VV<ll> &edges)
        : N(edges.size()), edges(edges), low(edges.size()), ord(edges.size()), 
          visited(edges.size()), is_articulation(edges.size()), dfs_counter(0)
    {
        for (ll i = 0; i < N; i++) if (!visited[i]) dfs(i, -1);
    }

    void dfs(ll cur, ll pre) {
        visited[cur] = true;
        ord[cur] = dfs_counter++;
        low[cur] = ord[cur];
        ll children = 0;
        bool art = false;
        for (ll nxt : edges[cur]) {
            if (!visited[nxt]) {
                children++;
                dfs(nxt, cur);
                chmin(low[cur], low[nxt]);
                art |= (pre == -1 ? 2 <= children : ord[cur] <= low[nxt]);
            } else if (nxt != pre) {
                chmin(low[cur], ord[nxt]);
            }
        }
        is_articulation[cur] = art;
    }

    bool is_art(ll cur) { return is_articulation[cur]; }

    // u -> v
    bool is_bridge(ll from, ll to) { return ord[from] < low[to]; }

    V<PLL> enum_bridges() {
        V<PLL> ret;
        for (ll from = 0; from < N; from++) {
            for (ll to : edges[from]) {
                if (!(from < to)) continue;
                if (is_bridge(from, to)) ret.emplace_back(from, to);
            }
        }
        return ret;
    }
};

int main() {
    ll N, M;
    cin >> N >> M;
    VV<ll> edges(N);
    for (ll i = 0; i < M; i++) {
        ll a, b;
        cin >> a >> b;
        edges[a].push_back(b);
        edges[b].push_back(a);
    }

    for (auto &&v : edges) sort(ALL(v));

    LowLink lowlink(edges);

    // for (ll i = 0; i < N; i++) if (lowlink.is_art(i)) cout << i << endl;
    for (ll i = 0; i < N; i++) for (ll j : edges[i]) {
        if (!(i < j)) continue;
        if (lowlink.is_bridge(i, j) || lowlink.is_bridge(j, i)) cout << i << ' ' << j << endl;
    }
    return 0;
}
