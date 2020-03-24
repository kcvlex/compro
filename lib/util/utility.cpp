#include "template.cpp"

ssize_t ceil_pow2(ssize_t s) {
    ssize_t ret = 1;
    while (ret <= s) ret *= 2;
    return ret;
}

ll ceil_div(ll a, ll b) {
    return a / b + !!(a % b);
}
