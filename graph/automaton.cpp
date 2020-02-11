/*
#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define ALL(V) (V).begin(), (V).end()
#define ALLR(V) (V).rbegin(), (V).rend()

template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
template <typename T, typename U> using P = pair<T, U>;
using ll = int64_t;
using PLL = P<ll, ll>;

template <typename T> const T& var_min(const T &t) { return t; }
template <typename T> const T& var_max(const T &t) { return t; }
template <typename T, typename... Tail> const T& var_min(const T &t, const Tail&... tail) { return min(t, var_min(tail...)); }
template <typename T, typename... Tail> const T& var_max(const T &t, const Tail&... tail) { return max(t, var_max(tail...)); }
template <typename T, typename... Tail> void chmin(T &t, const Tail&... tail) { t = var_min(t, tail...); }
template <typename T, typename... Tail> void chmax(T &t, const Tail&... tail) { t = var_max(t, tail...); }
template <typename T> const T& clamp(const T &t, const T &low, const T &high) { return max(low, min(high, t)); }
template <typename T> void chclamp(T &t, const T &low, const T &high) { t = clamp(t, low, high); }

template <typename T> T make_v(T init) { return init; }

template <typename T, typename... Tail>
auto make_v(T init, size_t s, Tail... tail) {
#define rec make_v(init, tail...)
    return V<decltype(rec)>(s, rec);
#undef rec
}
*/

template <size_t Q, size_t Label>
class Automaton {
    
    template <size_t S>
    struct BitsetComparator {
        bool operator()(const bitset<S> &s1, const bitset<S> &s2) {
            return s1.to_string() < s2.to_string();
        }
    };
    
public:
    using State = ll;
    using States = bitset<Q>;
    using StatesComparator = BitsetComparator<Q>;
    using StatesSet = set<States, StatesComparator>;
    using Edge = array<V<ll>, Label>;
    using Edges = V<Edge>;

    ll start;
    V<ll> finish_v;
    States finish_states;
    Edges edges;

    Automaton(ll start, V<ll> finish_v, Edges edges)
        : start(start),
          finish_v(finish_v),
          finish_states(vec2states(finish_v)),
          edges(edges) 
    {
    }

    Automaton(ll start, V<ll> finish_v)
        : Automaton(start, finish_v, Edges(Q)) {}

    void add_edge(ll now, ll nxt, ll label) {
        // cout << label << " : " << now << " -> " << nxt << endl;
        edges[now][label].push_back(nxt); 
    }

    Automaton<Q, Label> nfa2dfa() {
        StatesSet states_set;
        StatesSet finish_states_set;
        queue<States> que;
        map<States, array<StatesSet, Label>, StatesComparator> dfa_edges;
        que.push(start_state());

        states_set.insert(start_state());

        while (que.size()) {
            trans_states(states_set, finish_states_set, que, dfa_edges);
        }

        return move(create_dfa(states_set, finish_states_set, dfa_edges));
    }

private:
    // TODO: 前計算したほうが良いかもしれないね
    States vec2states(const V<ll> &finish_v) {
        States ret(0);
        for (ll e : finish_v) ret.set(e, 1);
        return move(ret);
    }

    States start_state() { return move(States(0).set(start, 1)); }

    States next_states(const States &now, ll label) {
        States ret(0);
        for (ll q = 0; q < Q; q++) {
            if (!now.test(q)) continue;
            for (auto nxt : edges[q][label]) {
                ret.set(nxt, 1);
            }
        }
        return move(ret);
    }

    void trans_states(StatesSet &states_set,
                      StatesSet &finish_states_set,
                      queue<States> &que, 
                      map<States, array<StatesSet, Label>, StatesComparator> &dfa_edges)
    {
        States now = que.front();
        que.pop();
        
        for (ll label = 0; label < Label; label++) {
            auto nxt = next_states(now, label);
            if (nxt == States(0)) continue;
            dfa_edges[now][label].insert(nxt);
            if (states_set.find(nxt) != states_set.end()) continue;
            states_set.insert(nxt);
            if ((nxt & finish_states).any()) finish_states_set.insert(nxt);
            que.push(nxt);
        }
    }

    map<States, ll, StatesComparator> create_states_id(const StatesSet &states_set) {
        map<States, ll, StatesComparator> states_id;
        for (const auto &states : states_set) {
            auto id = states_id.size();
            states_id[states] = id;
        }
        return move(states_id);
    }

    Automaton<Q, Label> create_dfa(const StatesSet &states_set,
                                   const StatesSet &finish_states_set,
                                   const map<States, array<StatesSet, Label>, StatesComparator> &dedges)
    {
        auto states_id = create_states_id(states_set);
        Edges dfa_edges(states_id.size());
        for (const auto &ele : dedges) {
            ll id = states_id[ele.first];
            for (ll label = 0; label < Label; label++) {
                for (auto next_states : ele.second[label]) {
                    dfa_edges[id][label].push_back(states_id[next_states]);
                }
            }
        }

        V<ll> dfa_finish_v;
        for (const auto &states : finish_states_set) {
            dfa_finish_v.push_back(states_id[states]);
        }
        sort(ALL(dfa_finish_v));

        ll start_id = states_id[start_state()];

        return move(Automaton<Q, Label>(start_id, dfa_finish_v, dfa_edges));
    }
};
