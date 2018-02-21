#include<bits/stdc++.h>
template <typename T>
namespace {
	using namespace std;
	class SegmentTree{
		public:
			int64_t n;
			T *data;
			T INF;  //comp(x, INF) == x
			T (*comp)(T, T);

			SegmentTree(int64_t size, T (*compArg)(T, T), T inf){
				int64_t newsize = 1;
				while(newsize < size) newsize = (newsize << 1);
				n = newsize;
				INF = inf;
				comp = compArg;
				data = new T[2 * n - 1];
				for(int64_t i = 0; i < 2 * n - 1; ++i) data[i] = inf;
			}
			
			void update(int64_t pos, T value){
				pos += n - 1;
				data[pos] = value;
				while(pos > 0){
					pos = (pos - 1) / 2;
					data[pos] = comp(data[2 * pos + 1], data[2 * pos + 2]);
				}
			}
			
			T query(int64_t a, int64_t b){
				return subQuery(a, b, 0, 0, n);
			}
			
			T subQuery(int64_t a, int64_t b, int64_t k, int64_t l, int64_t r){
				return (r <= a || b <= l ? INF : (a <= l && r <= b ? data[k] : comp(subQuery(a, b, k * 2 + 1, l, (l + r) / 2), subQuery(a, b, k * 2 + 2, (l + r) / 2, r))));
			}
	};
}
