#include "../../util/template.hpp"
#include "../../graph/flow-graph.hpp"
#include "../../flow/dinic.hpp"

int main() {
    const ll inf = 5e15;
    ll N, M;
    cin >> N >> M;
    graph::FlowGraph<true> graph(N);
    while (M--) {
        ll u, v, c;
        cin >> u >> v >> c;
        graph.add_edge(u, v, c);
    }

    flow::Dinic d(graph, inf);
    cout << d.max_flow(0, N - 1) << endl;
    return 0;
}
