#pragma once
#include "min-cost-b-flow.hpp"
#include "util/debug.hpp"

namespace flow {

template <typename Flow, typename Cost>
struct BFlowSSP : public MinCostBFlowSolverInterface<BFlowSSP, Flow, Cost> {
    using bflow_state_type = MinCostBFlow<BFlowSSP, Flow, Cost>;
    using edge_type = typename bflow_state_type::edge_type;

    BFlowSSP(bflow_state_type *bflow_state) : bflow_state(bflow_state) { }

    auto solve() -> std::pair<BFlowStatus, Cost> {
        initialize();
        while (dual()) primal();
        auto csum = bflow_state->cost_sum();
        if (excess.empty() && deficit.empty()) {
            return std::make_pair(BFlowStatus::Optimal, csum);
        } else {
            return std::make_pair(BFlowStatus::Infeasible, csum);
        }
    }
    
    auto get_state() {
        return bflow_state;
    }

private:
    vec<Cost> dists;
    vec<Node> excess, deficit;
    vec<edge_type*> prev_edges;
    bflow_state_type* bflow_state;

    Cost max_dist = 0;
    constexpr static Cost cinf = 5e15;

    void initialize() {
        for (auto &&ev : bflow_state->graph) for (auto &&e : ev) {
            auto rcap = bflow_state->residual_cap(e);
            auto rcost = bflow_state->residual_cost(e);
            if (rcap < 0 || rcost < 0) {
                bflow_state->push(e, rcap);
                bflow_state->b[e.src] -= rcap;
                bflow_state->b[e.dst] += rcap;
            }
        }

        excess.clear();
        deficit.clear();
        for (size_type i = 0; i < size_type(bflow_state->graph.size()); i++) {
            if (bflow_state->b[i] < 0) {
                deficit.push_back(i);
            } else if (0 < bflow_state->b[i]) {
                excess.push_back(i);
            }
        }
    }

    bool dijk() {
        using pq_ele = std::pair<Cost, Node>;
        
        std::fill(ALL(dists), cinf);
        std::priority_queue<pq_ele, vec<pq_ele>, std::greater<pq_ele>> pq;

        std::for_each(ALL(excess),
                      [&](Node n) {
                          dists[n] = 0;
                          pq.emplace(0, n);
                      });
        
        max_dist = 0;
        auto rest = size_type(deficit.size());

        while (!pq.empty()) {
            auto [ d, cur ] = pq.top();
            pq.pop();
            if (dists[cur] < d) continue;
            max_dist = d;
            if (bflow_state->b[cur] < 0) rest--;
            if (rest == 0) break;
            for (size_type i = 0; i < size_type(bflow_state->graph[cur].size()); i++) {
                auto &e = bflow_state->graph[cur][i];
                if (bflow_state->residual_cap(e) <= 0) continue;
                Node nxt = e.dst;
                Cost nd = d + bflow_state->residual_cost(e);
                if (dists[nxt] <= nd) continue;
                dists[nxt] = nd;
                pq.emplace(nd, nxt);
                prev_edges[nxt] = &(bflow_state->graph[cur][i]);
            }
        }

        for (size_type i = 0; i < size_type(dists.size()); i++) {
            bflow_state->potential[i] += std::min(dists[i], max_dist);
        }

        return rest < size_type(deficit.size());
    }

    bool dual() {
        dists.assign(bflow_state->graph.size(), cinf);
        prev_edges.assign(bflow_state->graph.size(), nullptr);

        excess.erase(std::remove_if(ALL(excess), [&](Node n) { return bflow_state->b[n] <= 0; }),
                     excess.end());
        deficit.erase(std::remove_if(ALL(deficit), [&](Node n) { return 0 <= bflow_state->b[n]; }),
                      deficit.end());

        return dijk();
    }

    std::pair<Flow, Node> calc_flow_amount(Node cur) const {
        Flow f = -1 * bflow_state->b[cur];
        for (auto e = prev_edges[cur]; e; e = prev_edges[e->src]) {
            chmin(f, bflow_state->residual_cap(*e));
            cur = e->src;
        }
        chmin(f, bflow_state->b[cur]);
        return std::make_pair(f, cur);
    }

    void push_flow(Node cur, Flow f) {
        for (auto e = prev_edges[cur]; e; e = prev_edges[e->src]) {
            bflow_state->push(*e, f);
            if (bflow_state->residual_cap(*e) <= 0) prev_edges[e->dst] = nullptr;
        }
    }

    void primal() {
        for (auto sink : deficit) {
            if (max_dist < dists[sink]) continue;
            auto [ f, src ] = calc_flow_amount(sink);
            if (f <= 0) continue;
            push_flow(sink, f);
            bflow_state->b[sink] += f;
            bflow_state->b[src] -= f;
        }
    }
};

}
