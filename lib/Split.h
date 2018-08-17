#include<bits/stdc++.h>
#include"Split.h"
using namespace std;

vector<string> split(const string &str, vector<string> sep){
    vector<string> ret;
    string now;
    sort(sep.begin(), sep.end(), [&](const string &a, const string &b){return a.size() > b.size();});
    for(int i = 0; i < str.size(); i++){
        bool is_sep = false;
        for(string se : sep){
            if(str.substr(i, se.size()) == se){
                if(now.size()){
                    ret.push_back(now);
                }
                now = "";
                ret.push_back(se);
                is_sep = true;
                i += se.size() - 1;
                break;
            }
        }
        if(!is_sep){
            now += str[i];
        }
    }
    if(now.size()){
        ret.push_back(now);
    }
    return ret;
}

vector<string> split(const string &str, vector<char> sep){
    vector<string> tmp;
    for(char s : sep){
        tmp.push_back(string(1, s));
    }
    return split(str, tmp);
}
