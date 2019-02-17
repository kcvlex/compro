#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
template <typename T> using V = vector<T>;
template <typename T> using VV = vector<V<T>>;

template <typename T>
class Bit {
private:
    V<T> data;
    T identity_ele;

public:
    Bit(ll size, T identity_ele) : identity_ele(identity_ele) {
        ll newsize = 1;
        while(newsize < size) {
            newsize = newsize << 1;
        }
        data = V<T>(newsize + 1, identity_ele);
    }

    /*
     * 0-origin
     * sum of [0, pos)
     */
    T sum(ll pos) {
        T ret = identity_ele;
        while(pos > 0) {
            ret += data[pos];
            pos -= pos & -pos;
        }
        return ret;
    }

    /*
     * 0-origin
     * sum of [l, r)
     */
    T sum(ll l, ll r) {
        return sum(r) - sum(l);
    }

    /*
     * 0-origin
     */
    void add(ll pos, T delta) {
        pos++;
        while(pos <= data.size()) {
            data[pos] = data[pos] + delta;
            pos += pos & -pos;
        }
    }
};

int main() {
    //TODO: test
    return 0;
}
