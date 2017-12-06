#include"segment.h"
#include<bits/stdc++.h>
using namespace std;
int mi(int a, int b){return (a > b ? b : a);}
int main(){
	SegmentTree<int> S(10, mi, 114514);
	for(int i = 0; i < 10; ++i){
		int a; cin >> a;
		S.update(i, a);
	}
	for(int i = 0; i < 10; ++i){
		for(int j = i + 1; j <= 10; ++j){
			cout << "[" << i + 1 << ", " << j + 1 << ")" << S.query(i, j) << endl;
		}
	}
	return 0;
}
