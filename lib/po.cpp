#include "UnionFind.h"
#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;

int main(){
    vector<ll> v;
    for(ll i = 0; i < 100; i++) v.push_back(i);
    Algo::UnionFind<ll> U(v);
    U.unit(10, 30);
    cout << (U.same(10, 30) ? "True\n" : "False\n");
    return 0;
}
