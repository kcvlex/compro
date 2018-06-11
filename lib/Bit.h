#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;
template <typename T>
class Bit{
    private:
        ll size;
        T *data;
        T identity_ele;
        T (*comp)(T, T);

    public:

        /*
         * size:            size
         * identity_ele:    単位元
         * comp:            比較関数
         */
        Bit(ll size, T identity_ele, T (*comp)(T, T)){
            ll newsize = 1;
            while(newsize < size)  newsize = newsize << 1;
            this->size = newsize + 1;
            this->identity_ele = identity_ele;
            this->comp = comp;
            data = new ll[size];
            for(ll i = 0; i < size; ++i){
                data[i] = identity_ele;
            }
        }

        /*
         * 0-origin
         */
        T sum(ll pos){
            pos++;
            T ret = identity_ele;
            while(pos > 0){
                ret += comp(ret, data[pos]);
                pos -= pos & -pos;
            }
            return ret;
        }

        /*
         * 0-origin
         */
        void add(ll pos, T delta){
            pos++;
            while(pos <= size){
                data[pos] = data[pos] + delta;
                pos += pos & -pos;
            }
        }
};
