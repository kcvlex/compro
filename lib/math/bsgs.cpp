#include "../util/template.cpp"
#include "base.cpp"
#include "modint.cpp"

namespace math {

namespace {

constexpr ll ceil_sqrt(ll n) {
    ll ok = n + 10, ng = 0;
    while (1 < ok - ng) {
        ll mid = (ok + ng) / 2;
        (n <= mid * mid ? ok : ng) = mid;
    }
    return ok;
}

DynModint make_modint(const DynModint &x, ll val) {
    return DynModint(val, x.mod());
}

template <ll Mod>
constexpr Modint<Mod> make_modint(const Modint<Mod> &x, ll val) {
    return Modint<Mod>(val);
}

}

template <typename T>
struct BSGS {
    constexpr BSGS(T x) : x(x), 
                          sqrt_mod__(ceil_sqrt(x.mod())),
                          inv_x_sqrt(pow(x, sqrt_mod__).inv())
    {
        T cur = make_modint(x, 1);
        for (ll i = 0; i < sqrt_mod__; i++, cur *= x) baby[cur.value()] = i;
    }

    ll solve(ll y) const {
        T cur = make_modint(x, 1);
        for (ll i = 0; i < sqrt_mod__; i++, cur *= inv_x_sqrt) {
            auto ite = baby.find(cur.value());
            if (ite != baby.end()) return sqrt_mod__ * i + (ite->second);
        }
        return -1;
    }

    ll sqrt_mod() const {
        return sqrt_mod__;
    }

    ll mod() const {
        return x.mod();
    }

private:
    const T x;
    const ll sqrt_mod__;
    const T inv_x_sqrt;
    std::unordered_map<ll, ll> baby;
};

}
