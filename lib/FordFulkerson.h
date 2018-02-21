#include<bits/stdc++.h>
namespace {
	using namespace std;
	class FordFulkerson{
		struct Edge{
			int64_t to, cap, rev_index;
		};
		private:
			vector<vector<Edge>> v;
			int64_t n;
			bool *used;
			const int64_t INF = numeric_limits<int64_t>::max();
			
			int dfs(int64_t now, int64_t goal, int64_t f){
				if(now == goal) return f;
				used[now] = 1;
				for(Edge &e : v[now]){
					if(used[e.to]){
						continue;
					}
					if(e.cap <= 0){
						continue;
					}
					int64_t d = dfs(e.to, goal, min(f, e.cap));
					if(d > 0){
						e.cap -= d;
						v[e.to][e.rev_index].cap += d;
						return d;
					}
				}
				return 0;
			}
		public:
			FordFulkerson(int64_t n, vector<Edge> *v){
				this->n = n;
				for(int64_t i = 0; i < n; i++){
					this->v.push_back(v[i]);
				}
				used = new bool[n];
			}
			
			FordFulkerson(int64_t n, vector<pair<int64_t, int64_t>> *v){
				this->n = n;
				for(int i = 0; i < n; i++){
					this->v.push_back(vector<Edge>(0));
				}
				for(int i = 0; i < n; i++){
					for(auto e : v[i]){
						this->v[i].push_back((Edge){e.first, e.second, (int64_t)this->v[e.first].size()});
						this->v[e.first].push_back((Edge){i, 0, (int64_t)this->v[i].size() - 1});
					}
				}
				used = new bool[n];
			}
			
			int64_t max_flow(int64_t start, int64_t goal){
				int64_t ret = 0;
				while(1){
					memset(used, 0, n);
					int64_t add = dfs(start, goal, INF);
					if(add == 0){
						return ret;
					}
					ret += add;
				}
			}
	};
}
