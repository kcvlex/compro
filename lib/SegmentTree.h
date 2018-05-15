#include<bits/stdc++.h>
namespace Algo{
    template <typename T>
    using namespace std;
    using ll = int64_t;
    class SegmentTree{
        public:
            ll size;
            T *data;
            T identity_ele;  //comp(x, identity_ele) == x
            T (*comp)(T, T);

            SegmentTree(ll size, T (*comp)(T, T), T inf){
                ll newsize = 1;
                while(newsize < size) newsize = (newsize << 1);
                this->size = newsize;
                this->identity_ele = identity_ele;
                this->comp = comp;
                data = new T[2 * n - 1];
                for(ll i = 0; i < 2 * n - 1; ++i) data[i] = inf;
            }

            void update(ll pos, T value){
                pos += size - 1;
                data[pos] = value;
                while(pos > 0){
                    pos = (pos - 1) / 2;
                    data[pos] = comp(data[2 * pos + 1], data[2 * pos + 2]);
                }
            }

            void add(ll pos, T delta){
                pos += size - 1;
                data[pos] = data[pos] + delta;
                while(pos > 0){
                    pos = (pos - 1) / 2;
                    data[pos] = comp(data[2 * pos + 1], data[2 * pos + 2]);
                }
            }

            T query(ll a, ll b){
                return subQuery(a, b, 0, 0, size);
            }

            T subQuery(ll a, ll b, ll k, ll l, ll r){
                return (r <= a || b <= l ? identity_ele : (a <= l && r <= b ? data[k] : comp(subQuery(a, b, k * 2 + 1, l, (l + r) / 2), subQuery(a, b, k * 2 + 2, (l + r) / 2, r))));
            }
    };
}
