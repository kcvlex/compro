#pragma once
#include "util/template.hpp"

namespace tree {

template <typename T>
class BIT {
    vec<T> data;

public:
    const T id_ele;

    BIT() : id_ele() { }

    BIT(size_type sz, T id_ele) : id_ele(id_ele) {
        data = vec<T>(ceil_pow2(sz) + 1, id_ele);
    }

    /*
    template <typename F>
    BIT(F f, size_type sz, T id_ele) : id_ele(id_ele) {
        data = vec<T>(ceil_pow2(sz) + 1, id_ele);
        for (size_type i = 0; i < sz; i++) {
            data[i + 1] = f(i);
            size_type par = i + (i & -i);
            if (par < size_type(data.size())) data[par] += data[i + 1];
        }
    }

    BIT(vec<T> d, T id_ele) : data(d), id_ele(id_ele) {
        size_type sz = data.size();
        data.resize(ceil_pow2(sz));
    }

    BIT(BIT &&obj) {
        data = std::move(obj.data);
        id_ele = std::move(obj.id_ele);
    }
    */

    // [0, pos)
    T sum(size_type pos) const noexcept {
        T ret = id_ele;
        for (; 0 < pos; pos -= pos & -pos) ret += data[pos];
        return ret;
    }

    // [l, r)
    T sum(size_type l, size_type r) const noexcept { 
        return sum(r) - sum(l); 
    }

    void add(ll pos, T delta) noexcept {
        for (++pos; pos < size_type(data.size()); pos += pos & -pos) data[pos] += delta;
    }

    // sum(ret) < bound <= sum(ret+1)
    size_type lower_bound(T bound) const noexcept {
        if (data.back() < bound) return data.size();
        T sum = id_ele;
        size_type ret = 0;
        for (size_type i = size_type(data.size() - 1) / 2; 0 < i; i /= 2) {
            if (sum + data[ret + i] < bound) {
                ret += i;
                sum += data[ret];
            }
        }
        return ret;
    }

    const vec<T>& raw() const {
        return data;
    }
};

}
