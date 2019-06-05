#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
template <typename T>
class SegmentTree {
public:
    ll size;
    vector<T> node;
    function<T(T, T)> comp;
    T identity_ele;  //comp(x, identity_ele) == x

    SegmentTree(const vector<T> &v, function<T(T, T)> comp, T identity_ele) 
        : comp(comp),
          identity_ele(identity_ele)
    {
        ll tmp = 1;
        while(tmp < v.size()) tmp = (tmp << 1);
        this->size = tmp;
        this->identity_ele = identity_ele;
        this->comp = comp;
        node = V<T>(2 * size - 1, identity_ele);
        for(ll i = 0; i < v.size(); i++) {
            node[i + size - 1] = v[i];
        }
        for(ll i = size - 2; 0 <= i; i--) {
            node[i] = comp(node[i * 2 + 1], node[i * 2 + 2]);
        }
    }

    void update(ll pos, T value) {
        pos += size - 1;
        node[pos] = value;
        while(pos > 0) {
            pos = (pos - 1) / 2;
            node[pos] = comp(node[2 * pos + 1], node[2 * pos + 2]);
        }
    }

    T query(ll a, ll b) {
        return sub_query(a, b, 0, 0, size);
    }

    T sub_query(ll a, ll b, ll k, ll l, ll r) {
        if(r <= a || b <= l) {
            return identity_ele;
        } else if(a <= l && r <= b) {
            return node[k];
        } else {
            T left = sub_query(a, b, k * 2 + 1, l, (l + r) / 2);
            T right = sub_query(a, b, k * 2 + 2, (l + r) / 2, r);
            return comp(left, right);
        }
    }
};
