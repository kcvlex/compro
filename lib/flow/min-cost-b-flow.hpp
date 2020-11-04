#pragma once
#include "util/template.hpp"
#include "flow/base.hpp"

namespace flow {

using NodeObject = NodeGenerator::Node_;
template <template <typename, typename> typename SolverImpl, typename Flow, typename Cost> struct MinCostBFlow;
template <template <typename, typename> typename SolverImpl, typename Flow, typename Cost> struct MinCostBFlowSolverInterface;

enum class BFlowStatus {
    Optimal, 
    Infeasible,
};

template <typename Flow, typename Cost>
struct BFlowEdge {
    Node src, dst;
    Flow flow;
    Flow cap;
    size_type rev_idx;
    Cost cost;

    BFlowEdge(Node src, Node dst, Flow cap, size_type rev_idx, Cost cost)
        : src(src), dst(dst), flow(0), cap(cap), rev_idx(rev_idx), cost(cost) 
    {
    }

    BFlowEdge() : BFlowEdge(0, 0, 0, 0, 0) { }
};

struct EdgeIndex {
    EdgeIndex(size_type idx1, size_type idx2) : idx1(idx1), idx2(idx2) { }
    EdgeIndex() : EdgeIndex(-1, -1) { }
private:
    size_type idx1, idx2;

    template <template <typename, typename> typename, typename, typename> friend class MinCostBFlow;
};

template <template <typename, typename> typename SolverImpl,
          typename Flow, 
          typename Cost>
struct MinCostBFlowSolverInterface {
    using solver_impl_type = SolverImpl<Flow, Cost>;

    auto solve() -> std::pair<BFlowStatus, Cost> {
        return static_cast<solver_impl_type*>(this)->solve();
    }

    auto solve(NodeObject src, NodeObject sink) -> std::tuple<BFlowStatus, Cost, Flow> {
        return solve_max_flow(src.v, sink.v);
    }

    template <typename... Args>
    static MinCostBFlowSolverInterface<SolverImpl, Flow, Cost>* make(Args&&... args) {
        auto ptr = new SolverImpl(std::forward<Args>(args)...);
        return static_cast<MinCostBFlowSolverInterface<SolverImpl, Flow, Cost>*>(ptr);
    }

    MinCostBFlow<SolverImpl, Flow, Cost>* get_state() {
        return static_cast<solver_impl_type*>(this)->get_state();
    }

protected:
    auto max_flow(Node src, Node sink) {
        max_flow_preprocess(src, sink);
        auto ret = solve(src, sink);
        max_flow_postprocess(src, sink);
        return ret;
    }

    void max_flow_preprocess(Node src, Node sink) {
        Cost inf_cap = std::abs(get_state()->b[src]);
        for (auto &e : get_state()->graph[src]) inf_cap += std::max<Flow>(e.cap, 0);
        add_edge(src, sink, 0, inf_cap, 0);
    }

    void max_flow_postprocess(Node src, Node sink) {
        get_state()->graph[src].pop_back();
        get_state()->graph[sink].pop_back();
    }
    
    auto solve_max_flow(Node src, Node sink) -> std::tuple<BFlowStatus, Cost, Flow> {
        auto [ status1, cost1 ] = solve();
        if (status1 == BFlowStatus::Infeasible) return std::make_tuple(status1, cost1, 0);

        Flow inf_flow = std::abs(get_state()->b[src]);
        for (auto &e : get_state()->graph[src]) inf_flow += get_state()->residual_cap(e);
        get_state()->b[src] += inf_flow;
        get_state()->b[sink] -= inf_flow;
        auto [ status2, cost2 ] = solve();
        get_state()->b[src] -= inf_flow;
        get_state()->b[sink] += inf_flow;
        return std::make_tuple(BFlowStatus::Optimal, cost2, get_state()->b[sink]);
    }
};

template <template <typename, typename> typename SolverImpl,
          typename Flow, 
          typename Cost>
struct MinCostBFlow {
    using impl_type = SolverImpl<Flow, Cost>;
    using edge_type = BFlowEdge<Flow, Cost>;
    using solver_type = MinCostBFlowSolverInterface<SolverImpl, Flow, Cost>;

    MinCostBFlow() {
        solver = std::move(std::unique_ptr<solver_type>(solver_type::make(this)));
    }

    const edge_type* access(EdgeIndex i) const {
        return &graph[i.idx1][i.idx2];
    }

    Flow residual_cap(const edge_type &e) const noexcept {
        return e.cap - e.flow;
    }

    Cost residual_cost(const edge_type &e) const noexcept {
        return e.cost + potential[e.src] - potential[e.dst];
    }

    NodeGenerator add_nodes(size_type sz) {
        auto cur = size_type(graph.size());
        resize(cur + sz);
        return NodeGenerator(cur, sz);
    }

    void add_suply(NodeObject n, Flow f) noexcept {
        b[n.v] += f;
    }

    void add_demand(NodeObject n, Flow f) noexcept {
        b[n.v] -= f;
    }

    auto add_edge(NodeObject src, NodeObject dst, Flow lower, Flow upper, Cost cost) {
        return add_edge(src.v, dst.v, lower, upper, cost);
    }

    auto solve() {
        return solver->solve();
    }

    auto solve(NodeObject src, NodeObject sink) {
        return solver->solve(src, sink);
    }

    const auto& get_graph() const noexcept {
        return graph;
    }

    const auto& get_potential() const noexcept {
        return potential;
    }

    size_type size() const noexcept {
        return graph.size();
    }

private:
    vvec<edge_type> graph;
    vec<Cost> potential, b;
    std::unique_ptr<solver_type> solver;

    void resize(size_type sz) {
        graph.resize(sz);
        b.resize(sz);
        potential.resize(sz);
    }

    void push(edge_type &e, Flow f) {
        e.flow += f;
        graph[e.dst][e.rev_idx].flow -= f;
    }
    
    EdgeIndex add_edge(Node src, Node dst, Flow lower, Flow upper, Cost cost) {
        size_type sidx = size_type(graph[src].size());
        size_type didx = size_type(graph[dst].size()) + (src == dst);

        graph[src].emplace_back(src, dst, upper, didx, cost);
        graph[dst].emplace_back(dst, src, -lower, sidx, -cost);
        return EdgeIndex(src, sidx);
    }
    
    Cost cost_sum() const {
        Cost ret = 0;
        for (auto &&ev : graph) for (auto &e : ev) ret += e.flow * e.cost;
        return ret / 2;
    }

    template <typename, typename> friend class BFlowSSP;
    template <typename, typename> friend class BFlowCapacityScaling;
    template <template <typename, typename> typename, typename, typename> friend class MinCostBFlowSolverInterface;
};

}
