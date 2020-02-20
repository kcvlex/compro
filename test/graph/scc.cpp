#include "../../graph/scc.cpp"

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_3_C&lang=jp
int main() {
    ll N, M;
    cin >> N >> M;
    graph::Graph<true> g(N);
    while (M--) {
        ll s, t;
        cin >> s >> t;
        g.add_edge(s, t);
    }

    SCC scc(g);
    auto group = scc.build();
    ll Q;
    cin >> Q;
    while (Q--) {
        ll u, v;
        cin >> u >> v;
        bool ans = (group[u] == group[v]);
        cout << (ans ? 1 : 0) << endl;
    }
    return 0;
}
