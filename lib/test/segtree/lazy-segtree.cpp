#include "../../util/template.hpp"
#include "../../segtree/lazy-segtree.hpp"

// solution for http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_F

const ll init_v = (1ll << 31) - 1;

struct Op {
    ll val;

    Op(ll val) : val(val) { }

    static Op id_ele() {
        return Op(init_v);
    }

    static Op merge(const Op &a, const Op &b) {
        return b;
    }
};

struct M {
    ll val;

    M(ll val) : val(val) { }

    static M id_ele() {
        return M(init_v);
    }

    static M merge(const M &a, const M &b) {
        return M(std::min(a.val, b.val));
    }

    void apply(Op op) {
        val = op.val;
    }
};

int main() {
    ll n, init = (1ll << 31) - 1;
    std::cin >> n;

    segtree::LazySegmentTree<M, Op> segs(vec<M>(n, M::id_ele()));

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
            std::cout << segs.get_query(s, t + 1).val << '\n';
        }
    }
    return 0;
}
