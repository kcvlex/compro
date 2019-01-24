#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;
using P = pair<ll, ll>;

template <int N>
bitset<N> independent_set(const vector<vector<ll>> &edges){
    ll fstl = 0, fstr = edges.size() / 2;
    ll scdl = fstr, scdr = edges.size();
    auto is_fst = [&](const ll n){ return fstl <= n && n < fstr; };
    auto align = [&](const ll n){ return is_fst(n) ? n : n - scdl; };
    vector<bitset<N>> allc, halfc;
    for(ll i = 0; i < edges.size(); i++){
        bitset<N> bsa(0), bsh(0);
        for(ll nxt : edges[i]){
            auto tmp = bitset<N>(1ll << nxt);
            bsa |= tmp;
            bool is_same = (is_fst(i) == is_fst(nxt));
            if(is_same) bsh |= bitset<N>(1ll << (align(nxt)));
        }
        allc.push_back(bsa);
        halfc.push_back(bsh);
    }
    vector<vector<bitset<N>>> dp = {
        vector<bitset<N>>(1ll << (fstr - fstl), bitset<N>(0)), 
        vector<bitset<N>>(1ll << (scdr - scdl), bitset<N>(0)),
    };
    for(ll i = 0; i < edges.size(); i++){
        ll which = !is_fst(i);
        ll now = align(i);
        ll upper = (1 << now);
        bitset<N> bs(1 << now);
        for(ll j = 0; j < upper; j++){
            bitset<N> subS = dp[which][j];
            bitset<N> update = (bitset<N>(j) | bs);
            dp[which][update.to_ullong()] = dp[which][j];
            for(ll k = 0; k < N / 2 + 1; k++){
                auto tmp = bitset<N>(j) & (~bitset<N>(1 << k));
                tmp = dp[which][tmp.to_ullong()];
                if((tmp & halfc[i]).any()) continue;
                tmp |= bs;
                if(dp[which][update.to_ullong()].count() < tmp.count()) dp[which][update.to_ullong()] = tmp; 
            }
        }
    }
    bitset<N> ret(0);
    for(ll i = 0; i < (1 << (fstr - fstl)); i++){
        bitset<N> bsi(dp[0][i]);
        bitset<N> nxts(0);
        for(ll d = 0; d < N; d++) if(bsi.test(d)) nxts |= allc[d];
        nxts = ~nxts;
        nxts <<= (N - scdr);
        nxts >>= (N - scdr);
        nxts >>= (fstr - fstl);
        bitset<N> tmp = bsi | (dp[1][nxts.to_ullong()] << (fstr - fstl));
        if(ret.count() < tmp.count()) ret = tmp;
    }
    return ret;
}

// slution for https://code-thanks-festival-2017.contest.atcoder.jp/tasks/code_thanks_festival_2017_g
ll N, M;
vector<string> ID;
vector<vector<ll>> fneeds(1e5 + 10);
vector<vector<ll>> edges(40);
bool bad[40][40];

int main(){
    ios::sync_with_stdio(false);
    cin >> N >> M;
    edges.resize(M);
    ll last1 = 0;
    for(ll i = 0; i < N; i++){
        ll n;
        cin >> n;
        if(n == 1){
            last1 = i;
        }else{
            string s;
            cin >> s;
            auto ite = find(ID.begin(), ID.end(), s);
            if(ite == ID.end()){
                ID.push_back(s);
            }
            ite = find(ID.begin(), ID.end(), s);
            ll num = ite - ID.begin();
            fneeds[last1].push_back(num);
        }
    }
    set<P> se;
    for(auto &v : fneeds){
        if(v.empty()) continue;
        sort(v.begin(), v.end());
        auto ite = unique(v.begin(), v.end());
        v.erase(ite, v.end());
        for(ll i = 0; i < v.size(); i++) for(ll j = 0; j < v.size(); j++) edges[v[i]].push_back(v[j]);
    }
    for(auto &v : edges){
        sort(v.begin(), v.end());
        auto ite = unique(v.begin(), v.end());
        v.erase(ite, v.end());
    }
    cout << independent_set<40>(edges).count() << endl;
    return 0;
}
