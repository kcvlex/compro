#pragma once
#include "../util/template.cpp"
#include "../math/fraction.cpp"
#include "base.cpp"

namespace geo {

/*
 *      3
 *    4 | 2
 * 5 ---0--- 1
 *    6 | 8
 *      7
 */

constexpr int pos_arr[3][3] = {
    { 4, 3, 2, },
    { 5, 0, 1, },
    { 6, 7, 8, },
};

struct RobustArg {
    math::fraction f;
    ll cycle;
    int pos;

    RobustArg(ll x, ll y, ll c) : f(y, x), cycle(c) {
        int tx = ternarizing(x) * -1;
        int ty = ternarizing(y) * -1;
        pos = pos_arr[ty + 1][tx + 1];
    }

    RobustArg(math::fraction f, ll c) : f(f), cycle(c) { }

    RobustArg(ll a, ll b) : RobustArg(a, b, 0) { }

    RobustArg() : RobustArg(0, 0, 0) { }

    constexpr bool operator ==(const RobustArg &oth) const {
        return f == oth.f && cycle == oth.cycle;
    }

    constexpr bool operator !=(const RobustArg &oth) const {
        return !(f == oth.f);
    }

    // WARNING : pos == 0
    constexpr bool operator <(const RobustArg &oth) const {
        if (cycle != oth.cycle) return cycle < oth.cycle;
        if (pos != oth.pos) return pos < oth.pos;
        if (pos & 1) return false;
        if ((pos / 2) & 1) return f < oth.f;  // pos is 1 or 3
        else return f > oth.f;
    }

    constexpr bool operator <=(const RobustArg &oth) const {
        return *this == oth || *this < oth;
    }

private:
    constexpr int ternarizing(ll n) const {
        if (n == 0) return 0;
        if (0 < n) return 1;
        return -1;
    }
};

RobustArg rotate(const RobustArg &arg, ll cnt) {
    auto ret = arg;
    ret.cycle += cnt;
    return ret;
}

RobustArg rotate(const RobustArg &arg, const RobustArg &base) {
    auto f1 = arg.f, f2 = base.f;
    auto den = math::fraction(1, 1) - (f1 * f2);
    auto num = f1 + f2;
    auto f = num / den;
}

RobustArg norm(const RobustArg &arg) {
    auto ret = arg;
    ret.cycle = 0;
    return ret;
}

RobustArg fixed_robust_arg(ll x, ll y) {
    RobustArg ret(x, y);
    if (y < 0) ret = rotate(ret, 1);
    return ret;
}

}
