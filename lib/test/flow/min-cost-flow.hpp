#include "../../util/template.hpp"
#include "../../graph/flow-graph.hpp"
#include "../../flow/min-cost-flow.hpp"

// solution for http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_6_B&lang=jp
int main() {
    ll n, e, f;
    std::cin >> n >> e >> f;
    graph::FlowGraph<true> fg(n);
    while (e--) {
        ll a, b, c, d;
        std::cin >> a >> b >> c >> d;
        fg.add_edge(a, b, c, d);
    }

    flow::MinCostFlow<decltype(fg)> mcf(fg);
    std::cout << mcf.solve(0, n - 1, f) << '\n';
    return 0;
}
