#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
class UnionFind{
    using vec = vector<ll>;
    public:
        vec rank;
        vec parent;

        UnionFind(ll N){
            rank = vec(N, 0);
            parent = vec(N);
            iota(parent.begin(), parent.end(), 0ll);
        }

        ~UnionFind(){
        }

        ll find(ll child){
            return (child == parent[child] ? child : parent[child] = find(parent[child]));
        }

        void unit(ll x, ll y){
            ll px = find(x);
            ll py = find(y);
            if(px == py){
                return;
            }
            if(rank[px] < rank[py]){
                swap(px, py);
            }
            parent[py] = px;
            rank[px] += (rank[px] == rank[py]);
        }

        bool same(ll x, ll y){
            return (find(x) == find(y));
        }

};
