#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;


struct GenNum {
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<ll> dis;

    GenNum(ll l, ll r)
        : rd(),
        gen(rd()),
        dis(l, r - 1) {}

    ll gen_num() {
        return dis(gen);
    }
};

int main() {
    GenNum gn(0, 7 + 1);
    for (ll i = 0; i < 30; i++) cout << gn.gen_num() << endl;
    return 0;
}
