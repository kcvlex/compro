#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;
template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
template <typename T, typename U> using P = pair<T, U>;
using PLL = P<ll, ll>;

class Dinic {
    struct Edge {
        ll to, cap, rev_idx;
    };
    
    VV<Edge> edges;
    const ll inf;

    V<ll> bfs(ll S) {
        V<ll> dist(edges.size(), -1);
        queue<PLL> que;
        que.emplace(S, 0);
        dist[S] = 0;
        while(que.size()) {
            ll now, d;
            tie(now, d) = que.front();
            que.pop();
            for(const auto &e : edges[now]) {
                if(e.cap <= 0) continue;
                ll nxt = e.to;
                if(dist[nxt] != -1 && dist[nxt] <= d + 1) continue;
                dist[nxt] = d + 1;
                que.emplace(nxt, d + 1);
            }
        }
        return move(dist);
    }

    ll dfs(ll now, ll pre, ll T, ll flow, const V<ll> &dist, V<ll> &watched) {
        if(now == T) return flow;
        for(ll i = watched[now] + 1; i < edges[now].size(); i++) {
            watched[now]++;
            auto &e = edges[now][i];
            if(dist[e.to] <= dist[now]) continue;
            if(e.cap <= 0) continue;
            if(e.to == pre) continue;
            ll f = dfs(e.to, now, T, min(flow, e.cap), dist, watched);
            e.cap -= f;
            edges[e.to][e.rev_idx].cap += f;
            if(f > 0) return f;
        }
        return 0;
    }

public: 
    Dinic(const VV<PLL> &v, ll inf) : inf(inf) {
        edges.resize(v.size());
        for(ll i = 0; i < v.size(); i++) {
            for(auto e : v[i]) {
                edges[i].push_back((Edge) {e.first, e.second, (ll)edges[e.first].size()} );
                edges[e.first].push_back((Edge) {i, 0, (ll)edges[i].size() - 1} );
            }
        }
    }

    Dinic(const VV<PLL> &v) : Dinic(v, 5e15) {};

    ll max_flow(ll S, ll T) {
        ll ret = 0;
        while(true) {
            auto dist = bfs(S);
            if(dist[T] == -1) break;
            V<ll> watched(edges.size(), -1);
            while(true) {
                ll tmp = dfs(S, -1, T, inf, dist, watched);
                if(tmp == 0) break;
                ret += tmp;
            }
        }
        return ret;
    }
};
