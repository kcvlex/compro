#include<bits/stdc++.h>
namespace Algo{
	using namespace std;
    using ll = int64_t;
    template <typename T>
	class UnionFind{
		public:
			vector<T> par;
			ll *rank;
            map<T, ll> id;

			UnionFind(vector<T> &par){
                this->par = par;
				rank = new T[par.size()];
				for(ll i = 0; i < par.size(); ++i){
					rank[i] = 0;
                    if(id.find(par[i]) != id.end()){
                        exit(1);
                    }
                    id[par[i]] = i;
				}
			}

			~UnionFind(){
			}

			T find(T child){
				return (child == par[id[child]] ? child : par[id[child]] = find(par[id[child]]));
			}

			void unit(T x, T y){
				x = find(x); y = find(y);
				if(x != y){
					if(rank[id[x]] > rank[id[y]]){
						par[id[y]] = x;
					}else{
						par[id[x]] = y;
						if(rank[id[x]] == rank[id[y]]){
							++rank[y];
						}
					}
				}
			}

			bool same(T x, T y){
				return (find(x) == find(y));
			}
	};
}
