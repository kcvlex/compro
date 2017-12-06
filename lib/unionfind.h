class UnionFind{
public:
    long long size;
    long long *par;
    long long *rank;

    UnionFind(long long);
    ~UnionFind();
    long long find(long long);
    void unit(long long, long long);
    bool same(long long, long long);
};

UnionFind::UnionFind(long long n){
    size = n;
    par = new long long[n];
    rank = new long long[n];
    for(int i = 0; i < n; ++i){
        par[i] = i;
        rank[i] = 0;
    }
}

UnionFind::~UnionFind(){
    delete[] par;
    delete[] rank;
}

long long UnionFind::find(long long child){
    return (child == par[child] ? child : par[child] = find(par[child]));
}

void UnionFind::unit(long long x, long long y){
    x = find(x); y = find(y);
    if(x != y){
        if(rank[x] > rank[y]){
            par[y] = x;
        }else{
            par[x] = y;
            if(rank[x] == rank[y]){
                ++rank[y];
            }
        }
    }
}

bool UnionFind::same(long long x, long long y){
    return (find(x) == find(y));
}
