#include "../../util/template.cpp"
#include "../../math/ntt.cpp"

// https://atcoder.jp/contests/atc001/tasks/fft_c
using namespace std;
int main() {
    math::NTT<1224736769, 17> ntt;
    int n;
    cin >> n;
    vec<ll> a(n + 1), b(n + 1);
    for (int i = 0; i < n; i++) cin >> a[i + 1] >> b[i + 1];
    auto conv = ntt.convolution(a, b);
    for (int i = 1; i <= 2 * n; i++) cout << conv[i] << endl;
    return 0;
}
