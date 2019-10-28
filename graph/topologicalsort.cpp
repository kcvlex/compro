#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;

vector<ll> top_sort(const vector<vector<ll>> &edges) {
    ll N = edges.size();
    vector<ll> ret;
    vector<bool> road(N);
    vector<bool> used(N);
    function<bool(ll)> dfs = [&](ll now) {
        used[now] = true;
        road[now] = true;
        for(ll nxt : edges[now]) {
            if(road[nxt]) return false;
            if(used[nxt]) continue;
            if(!dfs(nxt)) return false;
        }
        ret.push_back(now);
        road[now] = false;
        return true;
    };
    for(ll i = 0; i < edges.size(); i++) {
        if(used[i]) continue;
        if(!dfs(i)) return vector<ll>(0);
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

int main() {
    //TOOD: write test
}
