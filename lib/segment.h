#include<cstdint>

template <typename T>
class SegmentTree{
public:
    std::int64_t n;
    T *data;
	T INF;  //INF以外の任意のxについて、comp(x, INF) == x
	T (*comp)(T, T);

    SegmentTree(std::int64_t, T (*)(T, T), T);
    void update(std::int64_t, T);
    T query(std::int64_t, std::int64_t);
    T subQuery(std::int64_t, std::int64_t, std::int64_t, std::int64_t, std::int64_t);
};

template <typename T>
SegmentTree<T>::SegmentTree(std::int64_t size, T (*compArg)(T, T), T inf){
    std::int64_t newsize = 1;
    while(newsize < size) newsize = (newsize << 1);
    n = newsize;
	INF = inf;
	comp = compArg;
    data = new T[2 * n - 1];
    for(std::int64_t i = 0; i < 2 * n - 1; ++i) data[i] = inf;
}

template <typename T>
void SegmentTree<T>::update(std::int64_t pos, T value){
    pos += n - 1;
    data[pos] = value;
    while(pos > 0){
        pos = (pos - 1) / 2;
        data[pos] = comp(data[2 * pos + 1], data[2 * pos + 2]);
    }
}

template <typename T>
T SegmentTree<T>::query(std::int64_t a, std::int64_t b){
    return subQuery(a, b, 0, 0, n);
}

template <typename T>
T SegmentTree<T>::subQuery(std::int64_t a, std::int64_t b, std::int64_t k, std::int64_t l, std::int64_t r){
    return (r <= a || b <= l ? INF : (a <= l && r <= b ? data[k] : comp(subQuery(a, b, k * 2 + 1, l, (l + r) / 2), subQuery(a, b, k * 2 + 2, (l + r) / 2, r))));
}
