#include "../../graph/lowlink.hpp"

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
