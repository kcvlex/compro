class SegmentTree{
private:
    long long n;
    long long *data;

public:
    SegmentTree(long long);
    long long get(long long);
    void update(long long, long long);
    long long query(long long, long long);
    long long subQuery(long long, long long, long long, long long, long long);
};

SegmentTree::SegmentTree(long long size){
    long long newsize = 1;
    while(newsize < size) newsize = (newsize << 1);
    n = newsize;
    data = new long long[2 * n - 1];
    for(int i = 0; i < 2 * n - 1; ++i) data[i] = 1145148931919810;
}

void SegmentTree::update(long long pos, long long value){
    pos += n - 1;
    data[pos] = value;
    while(pos > 0){
        pos = (pos - 1) / 2;
        data[pos] = min<ll>(data[2 * pos + 1], data[2 * pos + 2]);
    }
}

long long get(long long i){
    return data[i];
}

long long SegmentTree::query(long long a, long long b){
    return subQuery(a, b, 0, 0, n);
}

long long SegmentTree::subQuery(long long a, long long b, long long k, long long l, long long r){
    return (r <= a || b <= l ? 1145148931919810 : (a <= l && r <= b ? data[k] : min<ll>(subQuery(a, b, k * 2 + 1, l, (l + r) / 2), subQuery(a, b, k * 2 + 2, (l + r) / 2, r))));
}
