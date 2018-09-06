#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
class UnionFind{
    public:
        ll size;
        ll *rank;
        ll *parent;

        UnionFind(ll N){
            this->size = N;
            rank = new ll[size];
            parent = new ll[size];
            for(ll i = 0; i < size; ++i){
                rank[i] = 0;
                parent[i] = i;
            }
        }

        ~UnionFind(){
        }

        ll find(ll child){
            return (child == parent[child] ? child : parent[child] = find(parent[child]));
        }

        void unit(ll x, ll y){
            x = find(x); y = find(y);
            if(x != y){
                if(rank[x] > rank[y]){
                    parent[y] = x;
                }else{
                    parent[x] = y;
                    if(rank[x] == rank[y]){
                        ++rank[y];
                    }
                }
            }
        }

        bool same(ll x, ll y){
            return (find(x) == find(y));
        }

};
