#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
class StrongDisassembly{

    private:
        ll V;
        vector<vector<ll>> E;
        vector<vector<ll>> invE;
        ll *number;
        ll toWriteNum = 0;

        void dfs(ll now, ll pre){
            for(ll next : E[now]){
                if(next == pre){
                    continue;
                }
                dfs(next, now);
            }
            number[now] = toWriteNum++;
        }

        void dfs(ll now, bool *used, vector<ll> &ret){
            used[now] = 1;
            ret.push_back(now);
            for(ll next : invE[now]){
                if(used[next]){
                    continue;
                }
                dfs(next, used, ret);
            }
        }

        void writeNum(){
            for(ll i = 0; i < V; i++){
                if(number[i] == -1){
                    dfs(i, -1);
                }
            }
        }

    public:
        StrongDisassembly(ll n, vector<ll> *e){
            V = n;
            number = new ll[n];
            for(ll i = 0; i < V; i++){
                E.push_back(vector<ll>(0));
                invE.push_back(vector<ll>(0));
            }
            for(ll i = 0; i < V; i++){
                for(ll next : e[i]){
                    E[i].push_back(next);
                    invE[next].push_back(i);
                }
            }
            writeNum();
        }

        map<ll, vector<ll>> disassembly(){
            map<ll, vector<ll>> ret;
            vector<ll> vec;
            bool used[V] = {};
            for(ll i = 0; i < V; i++){
                vec.push_back(i);
            }
            sort(vec.begin(), vec.end(), [&](ll a, ll b){
                    return number[a] > number[b];
                    }
                );
            for(auto vertex : vec){
                if(used[vertex]){
                    continue;
                }
                vector<ll> toInsert;
                dfs(vertex, used, toInsert);
                ret.insert(pair<ll, vector<ll>>((ll)ret.size() - 1, toInsert));
            }
            return ret;
        }
};
