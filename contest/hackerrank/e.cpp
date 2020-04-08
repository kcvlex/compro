#include "../../lib/util/template.cpp"
#include "../../lib/segtree/segtree.cpp"
const ll inf = 5e15;
void solve() {
    ll n;
    std::cin >> n;
    vec<ll> l(n), a(n), x(n);
    for (ll &e : l) {
        std::cin >> e;
        e--;
    }
    for (ll &e : a) std::cin >> e;
    for (ll &e : x) std::cin >> e;
    vec<ll> sum_o(n);
    for (ll i = 0; i < n; i++) {
        sum_o[i] += a[i];
        sum_o[l[i]] += a[i];
    }

    for (ll rion = 0; rion < n; rion++) {
        ll ok = 1e18, ng = -1;
        vec<pll> sum;
        for (ll i = 0; i < n; i++) {
            if (i == rion) continue;
            if (i == l[rion]) continue;
            sum.emplace_back(sum_o[i], i);
        }
        pll pinf(inf, inf);
        while (1 < std::abs(ok - ng)) {
            ll mid = (ok + ng) / 2;
            segtree::SegmentTree<pll> seg(sum, [](pll a, pll b) { return std::min(a, b); }, pinf);
            ll idx = 0;
            for (; idx < n - 2; idx++) {
                auto q = seg.get_query(0, n - 2);
                if (mid < q.first) break;
                ll i = l[q.second];
                auto qi = seg[i];
                if (qi != pinf) {
                    qi.first -= a[q.second];
                    seg.update_query(qi.second, qi);
                }
                seg.update_query(q.second, pinf);
            }
            (idx == n - 2 ? ok : ng) = mid;
        }
        std::cout << ok << " \n"[rion + 1 == n];
    }
}

int main() {
    ll t;
    std::cin >> t;
    while (t--) solve();
    return 0;
}
