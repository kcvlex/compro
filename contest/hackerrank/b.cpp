#include "../../lib/util/template.cpp"

const ll inf = 5e15;

ll solve() {
    ll n;
    std::cin >> n;
    auto dp = make_v<ll>(inf, n + 1, 7);
    std::fill(ALL(dp[0]), 0);
    vec<ll> a(n);
    for (ll &e : a) {
        std::cin >> e;
        e--;
    }

    for (ll i = 0; i < n; i++) {
        ll e = a[i];
        for (ll j = 0; j < 7; j++) for (ll k = -1; k <= 1; k++) {
            ll real = j + k;
            if (!(0 <= real && real < 7)) continue;
            chmin(dp[i + 1][real], dp[i][j] + (real != e));
        }
    }

    return *std::min_element(ALL(dp[n]));
}

int main() {
    ll t;
    std::cin >> t;
    while (t--) std::cout << solve() << '\n';
    return 0;
}
