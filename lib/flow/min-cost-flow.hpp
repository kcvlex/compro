#pragma once
#include "../util/template.cpp"
#include "../graph/flow-graph.cpp"

namespace flow {

using graph::Node;
using graph::Weight;
using graph::Capacity;

template <typename FlowGraph>
class MinCostFlow {
    using prev_node_t = std::pair<Node, ll>;
    using pq_ele_t = std::pair<Weight, Node>;
    FlowGraph fgraph;
    const Capacity cinf;
    const Weight dinf = 5e15;
    vec<Weight> dists, potential;
    vec<prev_node_t> pnode;

    void dijk(Node start) {
        std::priority_queue<pq_ele_t, vec<pq_ele_t>, std::greater<pq_ele_t>> pq;
        std::fill(ALL(dists), dinf);
        dists[start] = 0;
        pq.emplace(0, start);
        while (pq.size()) {
            Weight d;
            Node from;
            std::tie(d, from) = pq.top();
            pq.pop();
            if (dists[from] < d) continue;
            for (ll i = 0; i < fgraph[from].size(); i++) {
                const auto &edge = fgraph[from][i];
                auto to = edge.to();
                Weight dnxt = d + edge.weight() + potential[from] - potential[to];
                if (edge.cap() <= 0 || dists[to] <= dnxt) continue;
                dists[to] = dnxt;
                pnode[to] = prev_node_t(from, i);
                pq.emplace(dnxt, to);
            }
        }
    }

public:

    MinCostFlow(const FlowGraph &fgraph, const Capacity &cinf = 5e15)
        : fgraph(fgraph), cinf(cinf), dists(fgraph.size()), 
          potential(fgraph.size(), 0), pnode(fgraph.size())
    {
    }

    Weight solve(Node start, Node goal, Capacity flow) {
        Weight ret = 0;
        while (0 < flow) {
            dijk(start);
            if (dists[goal] == dinf) return -1;
            for (ll i = 0; i < fgraph.size(); i++) potential[i] += dists[i];

            Capacity max_flow = flow;
            for (Node cur = goal; cur != start; cur = pnode[cur].first) {
                Node pre;
                ll pidx;
                std::tie(pre, pidx) = pnode[cur];
                const auto &edge = fgraph[pre][pidx];
                chmin(max_flow, edge.cap());
            }

            if (max_flow == 0) return -1;
            flow -= max_flow;
            ret += max_flow * potential[goal];
            for (Node cur = goal; cur != start; cur = pnode[cur].first) {
                Node pre;
                ll pidx;
                std::tie(pre, pidx) = pnode[cur];
                auto &edge = fgraph[pre][pidx];
                edge.cap() -= max_flow;
                auto &redge = fgraph[cur][edge.rev_idx()];
                redge.cap() += max_flow;
            }
        }

        return ret;
    }
};

}
