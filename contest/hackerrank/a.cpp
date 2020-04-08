#include "../../lib/util/template.cpp"

ll input(vec<ll> &v) {
    for (ll &e : v) std::cin >> e;
    std::sort(ALL(v));
    ll cnt = 0;
    while (cnt < v.size() && v[cnt] == v[0]) cnt++;
    return cnt;
}

pll solve() {
    ll a, b, c;
    std::cin >> a >> b >> c;
    vec<ll> x(a), y(b), z(c);
    ll cx = input(x);
    ll cy = input(y);
    ll cz = input(z);

    return pll(x[0] + y[0] + z[0], cx * cy * cz);
}

int main() {
    ll t;
    std::cin >> t;
    while (t--) {
        ll a, b;
        std::tie(a, b) = solve();
        std::cout << a << ' ' << b << '\n';
    }
    return 0;
}
