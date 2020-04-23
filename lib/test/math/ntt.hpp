#include "../../util/template.hpp"
#include "../../math/ntt.hpp"

// https://atcoder.jp/contests/atc001/tasks/fft_c
int main() {
    math::NTT<1224736769, 17> ntt;
    int n;
    std::cin >> n;
    vec<ll> a(n + 1), b(n + 1);
    for (int i = 0; i < n; i++) std::cin >> a[i + 1] >> b[i + 1];
    auto conv = ntt.convolution(a, b);
    for (int i = 1; i <= 2 * n; i++) std::cout << conv[i].value() << '\n';
    return 0;
}
