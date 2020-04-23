#include "../../graph/scc.hpp"

// https://atcoder.jp/contests/arc030/tasks/arc030_3
using namespace std;
int main() {
    ll n, m, k;
    cin >> n >> m >> k;
    vec<char> alp(n);
    graph::Graph<true> graph(n);
    for (auto &&e : alp) cin >> e;
    for (ll i = 0; i < m; i++) {
        ll a, b;
        cin >> a >> b;
        graph.add_edge(a - 1, b - 1);
    }

    graph::scc<graph::Graph<true>> builder(graph);
    graph::Graph<true> cgraph;
    vec<ll> label;
    tie(cgraph, label) = builder.build_scc_graph();
    vvec<ll> nodes(cgraph.size());
    for (ll i = 0; i < n; i++) nodes[label[i]].push_back(i);
    const ssize_t SIZE = 310;
    string dp[SIZE][SIZE];
    for (ll i = 0; i < nodes.size(); i++) {
        priority_queue<char, vec<char>, greater<char>> pq;
        for (ll n : nodes[i]) pq.push(alp[n]);
        ll ok = 0;
        for (; ok < SIZE && dp[i][ok].size() == ok; ok++);
        for (ll maxv = ok; maxv < SIZE && pq.size(); maxv++) {
            char c = pq.top();
            pq.pop();
            for (ll j = maxv; 1 <= j; j--) {
                if (dp[i][j].size() == 0) dp[i][j] = dp[i][j - 1] + c;
                else chmin(dp[i][j], dp[i][j - 1] + c);
            }
        }
        for (auto &&e : cgraph[i]) {
            ll succ;
            tie(succ, ignore) = e;
            for (ll j = 1; j < SIZE && dp[i][j].size(); j++) {
                auto &v = dp[succ][j];
                if (v.size() == 0) dp[succ][j] = dp[i][j];
                else chmin(dp[succ][j], dp[i][j]);
            }
        }
    }

    const string inf(SIZE, 'z');
    string ans = inf;
    for (ll i = 0; i < SIZE; i++) if (dp[i][k].size() == k) chmin(ans, dp[i][k]);
    cout << (ans == inf ? "-1" : ans) << endl;
    return 0;
}
