#pragma once
#include "flow/min-cost-b-flow.hpp"
#include "util/debug.hpp"

namespace flow {

template <typename Flow, typename Cost>
struct BFlowCapacityScaling : public MinCostBFlowSolverInterface<BFlowCapacityScaling, Flow, Cost> {
    using bflow_state_type = MinCostBFlow<BFlowCapacityScaling, Flow, Cost>;
    using edge_type = typename bflow_state_type::edge_type;

    BFlowCapacityScaling(bflow_state_type *bflow_state) : bflow_state(bflow_state) { }

    auto solve() -> std::pair<BFlowStatus, Cost> {
        initialize();
        for (; delta; delta /= scaling_factor) {
            auto pred = [&](edge_type &e) {
                if (0 <= bflow_state->residual_cost(e)) return true;
                if (bflow_state->residual_cap(e) < delta) return true; 
                return false;
            };
            remove_unconditional_edges(pred);
            while (dual()) primal();
        }
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
    bflow_state_type *bflow_state;
    Cost delta;
    vec<Cost> dists;
    vec<Node> excess, deficit;
    vec<edge_type*> prev_edges;

    Cost max_dist = 0;
    constexpr static Cost cinf = 5e15;
    constexpr static Cost scaling_factor = 2;

    template <typename Predicate>
    void remove_unconditional_edges(Predicate pred) {
        // Push rcap when pred(e) is false
        for (auto &&ev : bflow_state->graph) for (auto &&e : ev) {
            auto rcap = bflow_state->residual_cap(e);
            if (!pred(e)) {
                bflow_state->push(e, rcap);
                bflow_state->b[e.src] -= rcap;
                bflow_state->b[e.dst] += rcap;
            }
        }
        split_st();
    }

    void split_st() {
        excess.clear();
        deficit.clear();
        for (size_type i = 0; i < bflow_state->size(); i++) {
            if (bflow_state->b[i] < 0) {
                deficit.push_back(i);
            } else if (0 < bflow_state->b[i]) {
                excess.push_back(i);
            }
        }
    }

    bool dijk() {
        using pq_ele = std::pair<Cost, Node>;
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
            if (bflow_state->b[cur] <= -delta) rest--;
            if (rest == 0) break;
            for (size_type i = 0; i < bflow_state->graph[cur].size(); i++) {
                auto e = &bflow_state->graph[cur][i];
                if (bflow_state->residual_cap(*e) < delta) continue;
                Node nxt = e->dst;
                Cost nd = d + bflow_state->residual_cost(*e);
                if (dists[nxt] <= nd) continue;
                dists[nxt] = nd;
                prev_edges[nxt] = e;
                pq.emplace(nd, nxt);
            }
        }

        for (size_type i = 0; i < bflow_state->size(); i++) {
            bflow_state->potential[i] += std::min(dists[i], max_dist);
        }

        return rest < size_type(deficit.size());
    }

    bool dual() {
        dists.assign(bflow_state->size(), cinf);
        prev_edges.assign(bflow_state->size(), nullptr);

        excess.erase(std::remove_if(ALL(excess), [&](Node n) { return bflow_state->b[n] < delta; }),
                     excess.end());
        deficit.erase(std::remove_if(ALL(deficit), [&](Node n) { return -delta < bflow_state->b[n]; }),
                      deficit.end());

        return dijk();
    }

    void initialize() {
        auto pred = [&](edge_type &e) { return 0 <= bflow_state->residual_cap(e); };
        remove_unconditional_edges(pred);

        Flow max_flow = 0;
        for (auto &&ev : bflow_state->graph) for (auto &&e : ev) chmax(max_flow, bflow_state->residual_cap(e));
        delta = 1;
        while (delta <= max_flow) delta *= scaling_factor;
    }
    
    std::pair<Flow, Node> calc_flow_amount(Node cur) const {
        Flow f = -1 * bflow_state->b[cur];
        for (auto e = prev_edges[cur]; e; e = prev_edges[e->src]) {
            auto rcap = bflow_state->residual_cap(*e);
            chmin(f, rcap);
            if (f < delta) break;
            cur = e->src;
        }
        chmin(f, bflow_state->b[cur]);
        return std::make_pair(f, cur);
    }

    void push_flow(Node cur, Flow f) {
        for (auto e = prev_edges[cur]; e; e = prev_edges[e->src]) {
            bflow_state->push(*e, f);
        }
    }

    void primal() {
        for (auto sink : deficit) {
            if (max_dist < dists[sink]) continue;
            auto [ f, src ] = calc_flow_amount(sink);
            if (f < delta) continue;
            push_flow(sink, f);
            bflow_state->b[sink] += f;
            bflow_state->b[src] -= f;
        }
    }
};

}
