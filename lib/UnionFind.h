#include<bits/stdc++.h>
namespace{
	using namespace std;
	class UnionFind{
		public:
			int64_t size;
			int64_t *par;
			int64_t *rank;

			UnionFind(int64_t n){
				size = n;
				par = new int64_t[n];
				rank = new int64_t[n];
				for(int i = 0; i < n; ++i){
					par[i] = i;
					rank[i] = 0;
				}
			}

			~UnionFind(){
				delete[] par;
				delete[] rank;
			}

			int64_t find(int64_t child){
				return (child == par[child] ? child : par[child] = find(par[child]));
			}

			void unit(int64_t x, int64_t y){
				x = find(x); y = find(y);
				if(x != y){
					if(rank[x] > rank[y]){
						par[y] = x;
					}else{
						par[x] = y;
						if(rank[x] == rank[y]){
							++rank[y];
						}
					}
				}
			}

			bool same(int64_t x, int64_t y){
				return (find(x) == find(y));
			}
	};
}
