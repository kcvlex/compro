#include<bits/stdc++.h>
namespace{
	class StrongDisassembly{

		private:
		int64_t V;
		vector<vector<int64_t>> E;
		vector<vector<int64_t>> invE;
		int64_t *number;
		int64_t toWriteNum = 0;

		void dfs(int64_t now, int64_t pre){
			for(int64_t next : E[now]){
				if(next == pre){
					continue;
				}
				dfs(next, now);
			}
			number[now] = toWriteNum++;
		}

		void dfs(int64_t now, bool *used, vector<int64_t> &ret){
			used[now] = 1;
			ret.push_back(now);
			for(int64_t next : invE[now]){
				if(used[next]){
					continue;
				}
				dfs(next, used, ret);
			}
		}

		void writeNum(){
			for(int64_t i = 0; i < V; i++){
				if(number[i] == -1){
					dfs(i, -1);
				}
			}
		}

		public:
		StrongDisassembly(int64_t n, vector<int64_t> *e){
			V = n;
			number = new int64_t[n];
			for(int64_t i = 0; i < V; i++){
				E.push_back(vector<int64_t>(0));
				invE.push_back(vector<int64_t>(0));
			}
			for(int64_t i = 0; i < V; i++){
				for(int64_t next : e[i]){
					E[i].push_back(next);
					invE[next].push_back(i);
				}
			}
			writeNum();
		}

		map<int64_t, vector<int64_t>> disassembly(){
			map<int64_t, vector<int64_t>> ret;
			vector<int64_t> vec;
			bool used[V] = {};
			for(int64_t i = 0; i < V; i++){
				vec.push_back(i);
			}
			sort(vec.begin(), vec.end(), [&](int64_t a, int64_t b){
					return number[a] > number[b];
					}
				);
			for(auto vertex : vec){
				if(used[vertex]){
					continue;
				}
				vector<int64_t> toInsert;
				dfs(vertex, used, toInsert);
				ret.insert(pair<int64_t, vector<int64_t>>((int64_t)ret.size() - 1, toInsert));
			}
			return ret;
		}
	};
}
