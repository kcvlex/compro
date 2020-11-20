#pragma once
#include "util/template.hpp"
#include "util/debug.hpp"

namespace graph {

template <typename Monoid>
struct OfflineDynamicConnectivity {
    enum class QueryType { Connect, Disconnect, Ask, };
    // using size_type = std::size_t;
    using edge_type = std::pair<int, int>;
    using query_type = std::pair<edge_type, QueryType>;

    static query_type make_connect(int u, int v) {
        return query_type(edge_type(u, v), QueryType::Connect);
    }

    static query_type make_disconnect(int u, int v) {
        return query_type(edge_type(u, v), QueryType::Disconnect);
    }

    static query_type make_ask(int v) {
        return query_type(edge_type(v, v), QueryType::Ask);
    }

    struct RollbackUnionFind {
        using rollback_t = std::tuple<int, int, bool, size_type, Monoid>;  // px, py, rank, sz, g
        vec<int> rank, par;
        vec<size_type> sz;
        vec<Monoid> mv;
        std::stack<rollback_t> history;

        template <typename F>
        RollbackUnionFind(F init, size_type n)
            : rank(n, 1), par(n), sz(n, 1), mv(n)
        {
            std::iota(ALL(par), 0ll);
            for (size_type i = 0; i < n; i++) mv[i] = std::move(init(i));
        }

        int find(int i) const noexcept {
            return (i == par[i] ? i : find(par[i]));
        }

        bool unit(int x, int y) {
            int px = find(x), py = find(y);
            if (px == py) return false;
            if (rank[px] < rank[py]) std::swap(px, py);
            bool add_rank = (rank[px] == rank[py]);
            history.emplace(px, py, add_rank, sz[px], mv[px]);
            par[py] = px;
            rank[px] += add_rank;
            sz[px] += sz[py];
            mv[px] = std::move(Monoid::merge(mv[px], mv[py]));
            return true;
        }

        void rollback() {
            const auto [ px, py, r, s, m ] = history.top();
            history.pop();
            mv[px] = std::move(m);
            sz[px] = s;
            rank[px] -= r;
            par[py] = py;
        }

        const Monoid& get_value(size_type i) const {
            return mv[find(i)];
        }

        size_type size(int x) const {
            return sz[find(x)];
        }
    };

    struct SimpleSegmentTree {
        vvec<size_type> nodes;
        const size_type sz;

        SimpleSegmentTree(size_type n) 
            : nodes(2 * ceil_pow2(n)), sz(nodes.size() / 2) 
        {
        }

        void add_edge_query(size_type query_idx, size_type l, size_type r) {
            chmin(r, sz);
            size_type lnode = l + sz, rnode = r + sz;
            while (lnode < rnode) {
                if (lnode & 1) {
                    nodes[lnode].push_back(query_idx);
                    lnode++;
                }
                if (rnode & 1) {
                    rnode--;
                    nodes[rnode].push_back(query_idx);
                }
                lnode /= 2; rnode /= 2;
            }
        }
    };

    const vec<query_type> &qv;
    SimpleSegmentTree qtree;
    RollbackUnionFind uf;
    std::map<edge_type, size_type> start;
    vec<Monoid> result;

    template <typename F>
    OfflineDynamicConnectivity(F init_value, 
                               const vec<query_type> &qv,
                               size_type n)
        : qv(qv),
          qtree(qv.size()),
          uf(init_value, n)
    {
        auto tmp = set_queries();
        result.reserve(tmp);
    }

    const vec<Monoid>& solve() {
        dfs(1, 0, qtree.sz);
        return result;
    }

private:
    edge_type fix_edge(edge_type e) {
        auto [ u, v ] = e;
        return std::minmax(u, v);
    }

    bool set_query(size_type t) {
        auto [ e, type ] = qv[t];
        e = fix_edge(e);

        if (type == QueryType::Connect) {
            if (start.find(e) != start.end()) return false;
            start.emplace(e, t);
        } else if (type == QueryType::Disconnect) {
            auto ite = start.find(e);
            if (ite == start.end()) return false;
            qtree.add_edge_query(ite->second, ite->second, t);
            start.erase(ite);
        } else if (type == QueryType::Ask) {
            return true;
        }
            
        return false;
    }

    size_type set_queries() {
        size_type ret = 0;
        for (size_type t = 0; t < qv.size(); t++) ret += set_query(t);
        for (auto [ e, t ] : start) qtree.add_edge_query(t, t, qtree.nodes.size());
        start.clear();
        return ret;
    }

    void dfs(size_type idx, size_type l, size_type r) {
        if (qv.size() <= l) return;

        size_type unit_cnt = 0;
        for (auto qi : qtree.nodes[idx]) {
            auto [ u, v ] = qv[qi].first;
            unit_cnt += uf.unit(u, v);
        }

        if (r - l == 1) {
            if (l < qv.size() && qv[l].second == QueryType::Ask) {
                auto [ u, v ] = qv[l].first;
                result.push_back(uf.get_value(u));
            }
        }

        if (1 < r - l) {
            size_type m = (l + r) / 2;
            dfs(2 * idx + 0, l, m);
            dfs(2 * idx + 1, m, r);
        }

        for (size_type i = 0; i < unit_cnt; i++) uf.rollback();
    }
};

}
