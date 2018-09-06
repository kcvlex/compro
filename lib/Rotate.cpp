#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;

template<typename T>
void rotate(vector<T> &v){
    vector<T> tmp(v[0].size());
    for(ll i = 0; i < v.size(); i++){
        for(ll j = 0; j < v[i].size(); j++){
            tmp[j].push_back(v[i][v[i].size() - j - 1]);
        }
    }
    v = tmp;
}
