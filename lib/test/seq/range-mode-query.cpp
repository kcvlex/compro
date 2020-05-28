#include "../../util/compressor.hpp"
#include "../../seq/range-mode-query.hpp"

auto naive(const vec<ll> &v, ll l, ll r) {
    std::map<ll, ll> cnts;
    for (ll i = l; i < r; i++) cnts[v[i]]++;
    ll ans = 0;
    for (auto &&e : cnts) chmax(ans, e.second);
    return ans;
}

int main() {
    int n, q;
    std::cin >> n >> q;
    vec<int> vn(n);
    for (int &e : vn) std::cin >> e;
    utility::Compressor<int> cmp(vn);
    auto cvn = cmp.compress<int>(vn);
    seq::RangeModeQuery rmq(cvn, cmp.size());
    while (q--) {
        int a, b;
        std::cin >> a >> b;
        b++;
        std::cout << rmq.query(a, b).second << "\n";
    }
    return 0;
}
