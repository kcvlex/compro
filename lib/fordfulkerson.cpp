#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
class FordFulkerson {
    struct Edge {
        ll to, cap, rev_index;
    } ;
private:
    vector<vector<Edge>> v;
    ll n;
    bool *used;
    const ll INF = numeric_limits<ll>::max();

    int dfs(ll now, ll goal, ll f) {
        if(now == goal) return f;
        used[now] = 1;
        for(Edge &e : v[now]) {
            if(used[e.to]) {
                continue;
            }
            if(e.cap <= 0) {
                continue;
            }
            ll d = dfs(e.to, goal, min(f, e.cap));
            if(d > 0) {
                e.cap -= d;
                v[e.to][e.rev_index].cap += d;
                return d;
            }
        }
        return 0;
    }

public:
    FordFulkerson(const vector<vector<pair<ll, ll>>> &v) {
        this->n = v.size();
        for(int i = 0; i < n; i++) {
            this->v.push_back(vector<Edge>(0));
        }
        for(int i = 0; i < n; i++) {
            for(auto e : v[i]) {
                this->v[i].push_back((Edge) {e.first, e.second, (ll)this->v[e.first].size()} );
                this->v[e.first].push_back((Edge) {i, 0, (ll)this->v[i].size() - 1} );
            }
        }
        used = new bool[n];
    }

    ll max_flow(ll start, ll goal) {
        ll ret = 0;
        while(1) {
            memset(used, 0, n);
            ll add = dfs(start, goal, INF);
            if(add == 0) {
                return ret;
            }
            ret += add;
        }
    }
} ;
