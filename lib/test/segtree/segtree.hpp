#include "../../util/template.hpp"
#include "../../segtree/segtree.hpp"
using namespace std;
int main() {
    ll n, q;
    cin >> n >> q;
    ll init = (1ll << 31) - 1;
    segtree::SegmentTree<ll> seg(vec<ll>(n, init), [](ll a, ll b) { return min(a, b); }, init);
    while (q--) {
        ll c, x, y;
        cin >> c >> x >> y;
        if (c == 0) seg.update_query(x, y);
        else cout << seg.get_query(x, y + 1) << endl;
    }
    return 0;
}
