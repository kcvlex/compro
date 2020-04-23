#include "../../util/template.cpp"
#include "../../segtree/lazysegtree.cpp"

// solution for http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F
int main() {
    ll n, init = (1ll << 31) - 1;
    std::cin >> n;

    auto merge = [](ll n1, ll n2) { return std::min(n1, n2); };
    auto apply = [](ll n, ll l) { return l; };
    auto update = [](ll n1, ll n2) { return n2; };
    auto calc_l = [](ll l, ll r, ll v) { return v; };
    segtree::LazySegmentTree<ll, ll> segs(
            vec<ll>(n, init), init, init, merge, apply, update, calc_l);

    ll q;
    std::cin >> q;
    while (q--) {
        ll qn, s, t;
        std::cin >> qn >> s >> t;
        if (qn == 0) {
            ll x;
            std::cin >> x;
            segs.update_query(s, t + 1, x);
        } else {
            std::cout << segs.get_query(s, t + 1) << '\n';
        }
    }
    return 0;
}
