#pragma once
#include "graph.hpp"

namespace graph {

template <typename Graph>
struct CycleDetector {
    enum class Status {
        Unreached,
        Searched,
        Searching,
    };

    const Graph &graph;
    std::stack<ll> stack;
    vec<Status> stv;
    bool found = false;

    CycleDetector(const Graph &graph) : graph(graph), stv(graph.size(), Status::Unreached) { }

    void dfs(ll cur, vec<ll> &ans) {
        stack.push(cur);
        stv[cur] = Status::Searching;
        for (auto nxt : graph::dst(graph[cur])) {
            if (stv[nxt] == Status::Searched) {
                continue;
            } else if (stv[nxt] == Status::Unreached) {
                dfs(nxt, ans);
                if (found) return;
            } else if (stv[nxt] == Status::Searching) {
                while (true) {
                    auto e = stack.top();
                    ans.push_back(e);
                    stack.pop();
                    if (e == nxt) break;
                }
                found = true;
                break;
            }
        }
        stv[cur] = Status::Searched;
        stack.pop();
    }

    vec<ll> solve() {
        vec<ll> ans;
        for (ll i = 0; i < graph.size(); i++) {
            if (stv[i] != Status::Unreached) continue;
            dfs(i, ans);
            if (ans.size()) break;
        }
        std::reverse(ALL(ans));
        return ans;
    }
};

template <typename Graph>
vec<ll> cycle_detect(const Graph &g) {
    CycleDetector cd(g);
    return cd.solve();
}

}
