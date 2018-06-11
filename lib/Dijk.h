#include<bits/stdc++.h>
class Dijk{
    using namespace std;
    using ll = int64_t;
    private:
        ll n;
        vector<pair<ll, ll>> v;
    public:
        const ll INF = INT64_MAX;
        
        Dijk(ll n, vector<pair<ll, ll>> *v){
            this->n = n;
            this->v = v;
        }
        
        vector<ll> dijk(ll start){
            vector<ll> d(n);
            for(ll i = 0; i < n; i++) d[i] = INF * (i != start);
            priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, functional<bool(pair<ll, ll>, pair<ll, ll>)>> q([&](pair<ll, ll> a, pair<ll, ll> b){return a.second < b.second;});
            q.push(pair<ll, ll>(start, 0));
            while(q.size()){
                pair<ll, ll> now = q.top();
                q.pop();
                d[now.first] = now.second;
                for(auto next : v){
                    if(d[next.first] == INF || d[now.first] + next.second < d[next.first]){
                        d[next.first] = d[now.first] + next.second;
                        q.push(pair<ll, ll>(next.first, d[now.first] + next.second));
                    }
                }
            }
            return d;
        }
};
