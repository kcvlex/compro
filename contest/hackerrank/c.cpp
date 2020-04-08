#include "../../lib/util/template.cpp"

ll shash[50][50];
bool used[50 * 50];
bool split[50];

ll solve() {
    ll n;
    std::string s;
    std::cin >> n >> s;
    if (n % 2 == 0) {
        std::string sa(n / 2, 'a'), sb(n / 2, 'b');
        auto ab = sa + sb;
        if (s == sa + sb) {
            ll ans = 0, sum = 0;
            for (ll i = 1;; i++) {
                if (sa.size() < sum + i) break;
                sum += i;
                ans++;
            }
            return ans * 2 - (sum == n / 2);
        }
    }
    vec<std::string> vs;
    
    {
        for (ll i = 0; i < n; i++) for (ll j = i + 1; j <= n; j++) vs.push_back(s.substr(i, j - i));
        std::sort(ALL(vs));
        auto ite = std::unique(ALL(vs));
        vs.erase(ite, vs.end());
    }
    
    for (ll i = 0; i < n; i++) for (ll j = i + 1; j <= n; j++) {
        auto ss = s.substr(i, j - i);
        shash[i][j] = std::distance(vs.begin(), std::lower_bound(ALL(vs), ss));
    }

    ll cnt = n / 2;
    split[0] = split[n] = true;
    ll ans = 0;

    for (ll S = 0; S < (1ll << cnt); S++) {
        bool center = false;
        std::fill(split + 1, split + n, false);

        auto split_f = [&](ll i) {
            split[i + 1] = true;
            split[n - (i + 1)] = true;
            if (i + 1 == n - (i + 1)) center = true;
        };

        for (ll i = 0; i < cnt; i++) if (S & (1ll << i)) {
            split_f(i);
        }

        ll cur = 0;
        bool ok = true;
        std::stack<ll> stk2;
        while (cur < n) {
            ll tmp = cur;
            cur++;
            while (!split[cur]) cur++;
            ll idx = shash[tmp][cur];
            if (used[idx]) {
                ok = false;
                break;
            }
            used[idx] = true;
            stk2.push(idx);
        }
        if (ok) {
            ll tmp = std::bitset<32>(S).count() * 2;
            if (center) tmp--;
            chmax(ans, tmp);
        }
        while (stk2.size()) {
            used[stk2.top()] = false;
            stk2.pop();
        }
    }

    return ans;
}

int main() {
    ll t;
    std::cin >> t;
    while (t--) std::cout << solve() << '\n';
    return 0;
}
