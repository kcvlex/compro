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

struct MinCostFlow {

    using To = ll;
    using Cap = ll;
    using Cost = ll;
    using RevIdx = ll;
    using Edge = tuple<To, Cap, Cost, RevIdx>;
    using PreNode = ll;
    using PreEdgeIdx = ll;
    using PrePathInfo = pair<PreNode, PreEdgeIdx>;
    using PathInfo = V<PrePathInfo>;
    using Distances = V<ll>;

    const ll vertex_num;
    VV<Edge> edges;
    V<ll> potential;
    const ll inf;


    MinCostFlow(ll vertex_num, ll inf = 5e15) 
        : vertex_num(vertex_num), 
          edges(vertex_num), 
          potential(vertex_num),
          inf(inf)
    {
    }

    void add_edge(ll from, ll to, ll cap, ll cost) {
        edges[from].emplace_back(to, cap, cost, edges[to].size());
        edges[to].emplace_back(from, 0, -cost, edges[from].size() - 1);
    }

    pair<Distances, PathInfo> dijk(ll start) {
        priority_queue<PLL, V<PLL>, greater<PLL>> pq;
        Distances dist(vertex_num, inf);
        PathInfo pi(vertex_num);
        dist[start] = 0;
        pq.emplace(0, start);
        while (pq.size()) {
            ll d, from;
            tie(d, from) = pq.top();
            pq.pop();
            if (dist[from] < d) continue;
            for (ll i = 0; i < edges[from].size(); i++) {
                ll to, cap, cost, rev_idx;
                tie(to, cap, cost, rev_idx) = edges[from][i];
                ll d_nxt = d + cost + potential[from] - potential[to];
                if (cap <= 0 || dist[to] <= d_nxt) continue;
                dist[to] = d_nxt;
                pi[to] = make_pair(from, i);
                pq.emplace(d_nxt, to);
            }
        }
        return move(make_pair(dist, pi));
    }

    ll& get_cap(Edge &edge) { return get<1>(edge); }
    ll& get_rev_idx(Edge &edge) { return get<3>(edge); }

    ll calc(ll start, ll goal, ll flow) {
        ll ret = 0;
        V<ll> dist(vertex_num, inf);
        while (flow > 0) {
            Distances dist;
            PathInfo pi;
            tie(dist, pi) = dijk(start);

            if (dist[goal] == inf) return -1;
            for (ll i = 0; i < vertex_num; i++) potential[i] += dist[i];
            
            ll max_flow = flow;
            for (ll now = goal; now != start; now = pi[now].first) {
                ll pre_node, pre_edge_idx;
                tie(pre_node, pre_edge_idx) = pi[now];
                auto &edge = edges[pre_node][pre_edge_idx];
                chmin(max_flow, get_cap(edge));
            }

            if (max_flow == 0) return -1;

            flow -= max_flow;
            ret += max_flow * potential[goal];

            for (ll now = goal; now != start; now = pi[now].first) {
                ll pre_node, pre_edge_idx;
                tie(pre_node, pre_edge_idx) = pi[now];
                auto &edge = edges[pre_node][pre_edge_idx];
                get_cap(edge) -= max_flow;
                auto &rev_edge = edges[now][get_rev_idx(edge)];
                get_cap(rev_edge) += max_flow;
            }
        }

        return ret;
    }
};

// solution for http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_B&lang=jp
int main() {
    ll N, E, F;
    cin >> N >> E >> F;
    MinCostFlow mcf(N);
    for (ll i = 0; i < E; i++) {
        ll a, b, cap, cost;
        cin >> a >> b >> cap >> cost;
        mcf.add_edge(a, b, cap, cost);
    }

    cout << mcf.calc(0, N - 1, F) << endl;
    return 0;
}
