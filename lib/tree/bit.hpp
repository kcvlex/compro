#pragma once
#include "util/template.hpp"

namespace tree {

template <typename T>
class BIT {
    vec<T> data;
    T id_ele;

public:
    BIT(size_type sz, T id_ele) : id_ele(id_ele) {
        data = vec<T>(ceil_pow2(sz) + 1, id_ele);
    }

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
};

}
