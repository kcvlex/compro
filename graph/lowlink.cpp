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
        for(ll i = 0; i < N; i++) if(!visited[i]) dfs(i, -1);
    }

    ll dfs(ll cur, ll pre) {
        visited[cur] = true;
        ord[cur] = dfs_counter++;
        low[cur] = ord[cur];
        ll children = 0;
        bool art = false;
        for(ll nxt : edges[cur]) {
            if(!visited[nxt]) {
                children++;
                ll res = dfs(nxt, cur);
                chmin(low[cur], low[nxt]);
                art |= (pre == -1 ? 2 <= children : ord[cur] <= low[nxt]);
            } else {
                chmin(low[cur], ord[nxt]);
            }
        }
        is_articulation[cur] = art;
        return ord[cur];
    }

    bool is_art(ll cur) { return is_articulation[cur]; }

    // u -> v
    bool is_bridge(ll from, ll to) { return ord[from] < low[to]; }

    V<PLL> enum_bridges() {
        V<PLL> ret;
        for(ll from = 0; from < N; from++) {
            for(ll to : edges[from]) {
                if(!(from < to)) continue;
                if(is_bridge(from, to)) ret.emplace_back(from, to);
            }
        }
        return ret;
    }
};
