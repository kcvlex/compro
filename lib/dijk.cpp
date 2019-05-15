#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;

template<typename T>
class Dijk {
private:
    template <typename U> using Edge = pair<ll, U>;
    template <typename U> using Edges = vector<Edge<U>>;
    template <typename U> using PQ = priority_queue<pair<ll, U>, vector<pair<ll, U>>, function<bool(pair<ll, U>, pair<ll, U>)>>;
    
    const vector<Edges<T>> &edges;
    T inf;
    T identity_ele;
        
public:
    Dijk(const vector<Edges<T>> &edges, T inf, T identity_ele)
        : edges(edges),
          inf(inf),
          identity_ele(identity_ele) {}

    vector<T> dijk(ll start) {
        vector<T> d(edges.size(), inf);
        d[start] = identity_ele;
        PQ<T> q([&](const Edge<T> &a, const Edge<T> &b) {return a.second > b.second;});
        q.push(make_pair(start, 0));
        while(q.size()) {
            ll now;
            T dis;
            tie(now, dis) = q.top();
            q.pop();
            if(d[now] < dis) {
                continue;
            }
            for(const auto &e : edges[now]) {
                ll nxt;
                T cost;
                tie(nxt, cost) = e;
                if(d[nxt] == inf || dis + cost < d[nxt]) {
                    d[nxt] = dis + cost;
                    q.push(make_pair(nxt, dis + cost));
                }
            }
        }
        return d;
    }
};

int main() {
    //TODO: test
    return 0;
}
