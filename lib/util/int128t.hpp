#include "template.hpp"

using i128 = __int128_t;

std::istream& operator >>(std::istream &is, i128 &n) {
    ll i;
    is >> i;
    n = i;
    return is;
}

std::ostream& operator <<(std::ostream &os, i128 n) {
    i128 maxv = std::numeric_limits<ll>::max();
    ll a = dynamic_cast<ll>(n / maxv);
    ll b = dynamic_cast<ll>(n % maxv);
    if (a) os << a;
    os << b;
    return os;
}
