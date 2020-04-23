#include "../util/template.hpp"

namespace tree {

template <typename T>
struct BIT {
    vec<T> data;
    T id_ele;

    BIT(ll size, T id_ele) : id_ele(id_ele) {
        ll bsize = 1;
        while (bsize < size) bsize *= 2;
        data = vec<T>(bsize + 1, id_ele);
    }

    T sum(ll pos) {
        T ret = id_ele;
        for (; 0 < pos; pos -= pos & -pos) ret += data[pos];
        return ret;
    }

    T sum(ll l, ll r) { return sum(r) - sum(l); }

    void add(ll pos, T delta) {
        for (++pos; pos < data.size(); pos += pos & -pos) data[pos] += delta;
    }
};

}
