#include<bits/stdc++.h>
using namespace std;

int main(){
    int n, t; cin >> n >> t;
    int ans = 0;
    int a[n + 1];
    for(int i = 0; i < n; ++i) cin >> a[i];
    a[n] = 1145141919;
    for(int i = 0; i < n; ++i) ans += min(t, a[i + 1] - a[i]);
    cout << ans << endl;
    return 0;
}
