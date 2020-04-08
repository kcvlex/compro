#include "../../lib/util/template.cpp"
#include "../../lib/tree/bit.cpp"

struct Compress {
    vec<ll> v;

    Compress(const vec<ll> &v_) : v(v_) {
        std::sort(ALL(v));
        auto ite = std::unique(ALL(v));
        v.erase(ite, v.end());
    }

    ll get_idx(ll x) {
        return std::distance(v.begin(),
                             std::lower_bound(ALL(v), x));
    }
};

void solve() {
    ll n, q, k;
    std::cin >> n >> q >> k;
    vec<ll> a(2 * n);
    for (ll &e : a) std::cin >> e;
    vec<tll> lr(q);
    for (auto &&e : lr) {
        ll l, r, x;
        std::cin >> l >> r >> x;
        e = tll(l - 1, r, x);
    }

    auto cmp = [&] {
        auto ev = a;
        auto cur = a;
        for (auto &&e : lr) {
            ll l, r, x;
            std::tie(l, r, x) = e;
            for (ll i = l; i < r; i++) {
                (cur[i] += x) %= k;
                ev.push_back(cur[i]);
            }
        }
        return Compress(ev);
    }();

    ll maxv = cmp.v.size();
    tree::BIT<ll> cnt_o(maxv + 1, 0), cnt_e(maxv + 1, 0), sum_o(maxv + 1, 0), sum_e(maxv + 1, 0);
    for (ll i = 0; i < 2 * n; i++) {
        ll id = cmp.get_idx(a[i]);
        auto &cbit = (i & 1 ? cnt_o : cnt_e);
        auto &sbit = (i & 1 ? sum_o : sum_e);
        cbit.add(id, 1);
        sbit.add(id, a[i]);
    }

    auto f = [&](ll b) {
        auto lb = cmp.get_idx(b);
        ll ce = cnt_e.sum(0, lb), se = sum_e.sum(0, lb);
        ll co = cnt_o.sum(lb, maxv), so = sum_o.sum(lb, maxv);
        return ce * b - se + so - co * (b - 1);
    };

    auto cur = a;
    for (ll loop = 0; loop < q; loop++) {
        ll ans = 0;
        ll l, r, x;
        std::tie(l, r, x) = lr[loop];
        for (ll i = l; i < r; i++) {
            ll pre = cur[i];
            ll nxt = (cur[i] + x) % k;
            ll pid = cmp.get_idx(pre);
            ll nid = cmp.get_idx(nxt);
            auto &cbit = (i & 1 ? cnt_o : cnt_e);
            auto &sbit = (i & 1 ? sum_o : sum_e);
            cbit.add(pid, -1);
            sbit.add(pid, -pre);
            cbit.add(nid, 1);
            sbit.add(nid, nxt);

            ll lv = 0, rv = cmp.v.back();
            while (2 < rv - lv) {
                ll x1 = (2 * lv + rv) / 3;
                ll x2 = (lv + 2 * rv) / 3;
                if (f(x1) < f(x2)) rv = x2;
                else lv = x1;
            }

            ll res = var_min(f(lv), f(lv + 1), f(lv + 2));
            ans += res;
            cur[i] = nxt;
        }

        std::cout << ans << " \n"[loop + 1 == q];
    }
}

int main() {
    ll t;
    std::cin >> t;
    while (t--) solve();
    return 0;
}
