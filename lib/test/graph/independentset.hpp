#include "../../util/template.cpp"
#include "../../graph/graph.cpp"
#include "../../graph/independentset.cpp"

// slution for https://code-thanks-festival-2017.contest.atcoder.jp/tasks/code_thanks_festival_2017_g
int main() {
    int n, m;
    std::cin >> n >> m;
    graph::Graph<false> g(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        std::cin >> a >> b;
        g.add_edge(a - 1, b - 1);
    }

    std::cout << graph::indep_set(g) << '\n';
    return 0;
}
