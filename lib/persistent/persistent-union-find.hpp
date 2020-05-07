#pragma once
#include "../util/template.hpp"

namespace persistent {

using size_type = ssize_t;

template <typename T>
struct nodes_pool {
    using array_type = vec<T>*;
    using diff_type = std::tuple<size_type, T, size_type>;
    using node_type = std::variant<array_type, diff_type>;

    nodes_pool<T>() { }

    template <typename F>
    void build(size_type sz, F f) {
        body.resize(sz);
        for (size_type i = 0; i < sz; i++) body[i] = f(i);
        pool.emplace_back(array_type(&body));
    }

    T get(size_type cur, size_type i) {
        reroot(cur);
        auto &arr = **std::get_if<array_type>(&(pool[cur]));
        return arr[i];
    }

    size_type set(size_type cur, size_type i, T v) {
        if (get(cur, i) == v) return cur;
        auto &arr = **std::get_if<array_type>(&(pool[cur]));  // pool[cur] must be array_type
        auto nxt = pool.size();
        auto old = std::move(arr[i]);
        pool[cur] = diff_type(i, std::move(old), nxt);
        arr[i] = std::move(v);
        pool.emplace_back(array_type(&arr));
        return nxt;
    }

private:
    vec<node_type> pool;
    vec<T> body;

    void reroot(size_type cur) {
        if (auto ptr = std::get_if<diff_type>(&(pool[cur]))) {
            auto [ i, v, nxt ] = *ptr;
            reroot(nxt);
            auto arr_ptr = *std::get_if<array_type>(&(pool[nxt]));  // pool[nxt] must be array_type
            auto &arr = *arr_ptr;
            auto old = std::move(arr[i]);
            arr[i] = std::move(v);
            pool[nxt] = diff_type(i, std::move(old), cur);
            pool[cur] = array_type(arr_ptr);
        }
    }
};

template <typename T>
struct PersistentArray {
    using pool_type = nodes_pool<T>;
    using pool_ptr = std::shared_ptr<pool_type>;

    PersistentArray<T>(size_type idx, pool_ptr pool) : idx(idx), pool(pool) { }
    PersistentArray<T>() : PersistentArray<T>(-1, pool_ptr()) { }

    template <typename F>
    static PersistentArray build(size_type sz, F f) {
        auto p = pool_ptr(new pool_type);
        p->build(sz, f);
        return PersistentArray(0, p);
    }

    const T get(size_type i) {
        return pool->get(idx, i);
    }

    PersistentArray set(size_type i, T v) {
        auto nxt = pool->set(idx, i, v);
        return PersistentArray(nxt, pool);
    }

private:
    size_type idx;
    pool_ptr pool;
};

struct PersistentUnionFind {
    using array_type = PersistentArray<size_type>;

    PersistentUnionFind(array_type par, array_type rank) : par(par), rank(rank) { }
    PersistentUnionFind() : PersistentUnionFind(array_type(), array_type()) { }

    static PersistentUnionFind build(size_type sz) {
        auto make_par = [](size_type i) { return i; };
        auto make_rank = [](size_type i) { return 1; };
        auto p = array_type::build<decltype(make_par)>(sz, make_par);
        auto r = array_type::build<decltype(make_rank)>(sz, make_rank);
        return PersistentUnionFind(p, r);
    }

    auto find(size_type v) {
        auto p = par.get(v);
        if (p == v) {
            return v;
        } else {
            p = find(p);
            par = par.set(v, p);
            return p;
        }
    }

    PersistentUnionFind unit(size_type x, size_type y) {
        x = find(x);
        y = find(y);
        
        if (x == y) return *this;

        auto rx = rank.get(x);
        auto ry = rank.get(y);
        if (rx < ry) {
            std::swap(rx, ry);
            std::swap(x, y);
        }
        auto next_par = par.set(y, x);
        auto next_rank = (rx == ry ? rank.set(x, rx + 1) : rank);
        return PersistentUnionFind(next_par, next_rank);
    }

    bool is_same(size_type x, size_type y) {
        return find(x) == find(y);
    }

private:
    array_type par;
    array_type rank;
};

}
