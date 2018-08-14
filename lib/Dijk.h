#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
class Dijk{
    private:
        ll n;
        vector<vector<pair<ll, T>>> v;
    public:
        T INF;
        
        Dijk(const vector<vector<pair<ll, T>>> &v, T INF){
            this->n = v.size();
            this->v = v;
            this->INF = INF;
        }
        
        vector<ll> dijk(ll start){
            vector<ll> d(n, INF);
            d[start] = 0;
            priority_queue<pair<ll, T>, vector<pair<ll, T>>, function<bool(pair<ll, T>, pair<ll, T>)>> q([&](pair<ll, T> a, pair<ll, T> b){return a.second > b.second;});
            q.push(pair<ll, T>(start, 0));
            while(q.size()){
                pair<ll, T> now = q.top();
                q.pop();
                if(d[now.first] < now.second){
                    continue;
                }
                for(auto next : v[now.first]){
                    if(d[next.first] == INF || d[now.first] + next.second < d[next.first]){
                        d[next.first] = d[now.first] + next.second;
                        q.push(pair<ll, T>(next.first, d[now.first] + next.second));
                    }
                }
            }
            return d;
        }
};
