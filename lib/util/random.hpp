#pragma once
#include "template.hpp"

// FIXME
struct RandomGen {
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<ll> dis;

    // [l, r)
    RandomGen(ll l, ll r)
        : rd(), mt(rd()), dis(l, r - 1)
    {
    }

    ll gen() {
        return dis(mt);
    }
};
