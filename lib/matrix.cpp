#include<bits/stdc++.h>
using namespace std;
using ll = int64_t;

template <size_t N, size_t M, typename T = ll>
struct Matrix {
    using Column = array<T, M>;
    using Mat = Matrix<N, M>;

    array<Column, N> mat;
    const T init;
    
    Matrix(const decltype(mat) &mat, const T &init) : mat(mat), init(init) {}

    Matrix(const T &init) : init(init) {
        Column initc;
        fill(initc.begin(), initc.end(), init);
        fill(mat.begin(), mat.end(), initc);
    }
    
    Mat operator + (const Mat &other) const {
        Mat ret(this->mat);
        for(ll i = 0; i < N; i++){
            for(ll j = 0; j < M; j++){
                ret.mat[i][j] += other.mat[i][j];
            }
        }
        return ret;
    }

    Mat& operator += (const Mat &other) {
        this->mat = ((*this) + other).mat;
        return (*this);
    }

    Mat operator - (const Mat &other) const {
        Mat ret(this->mat);
        for(ll i = 0; i < N; i++){
            for(ll j = 0; j < M; j++){
                ret.mat[i][j] -= other.mat[i][j];
            }
        }
        return ret;
    }

    Mat& operator -= (const Mat &other) {
        this->mat = ((*this) - other).mat;
        return (*this);
    }

    template <size_t K>
    Matrix<N, K> operator * (const Matrix<M, K> &other) const {
        Mat ret(init);
        for(ll n = 0; n < N; n++){
            for(ll k = 0; k < K; k++){
                T tmp = init;
                for(ll i = 0; i < M; i++){
                    tmp += (this->mat)[n][i] * other.mat[i][k];
                }
                ret.mat[n][k] = tmp;
            }
        }
        return ret;
    }

    template <size_t K>
    Matrix<N, K>& operator *= (const Matrix<M, K> &other) {
        this->mat = ((*this) * other).mat;
        return (*this);
    }
};

int main(){
    array<array<ll, 3>, 3> arr = {
        array<ll, 3>{1, 2, 3},
        array<ll, 3>{4, 5, 6},
        array<ll, 3>{7, 8, 9},
    };
    Matrix<3, 3> ma(arr, 0);
    auto tmp = ma * ma;
    auto mat = tmp.mat;
    for(ll i = 0; i < mat.size(); i++){ for(ll j = 0; j < mat[i].size(); j++) cout << mat[i][j] << ", "; cout << endl; }
//    (ma *= ma) *= ma;
    ma *= ma;
    mat = ma.mat;
    for(ll i = 0; i < mat.size(); i++){ for(ll j = 0; j < mat[i].size(); j++) cout << mat[i][j] << ", "; cout << endl; }
    return 0;
}
