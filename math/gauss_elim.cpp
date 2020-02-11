#include <bits/stdc++.h>
using namespace std;
using ll = int64_t;
template <typename T> using V = vector<T>;
template <int N> using BS = bitset<N>;
template <int N> using Matrix = V<BS<N>>;

template <size_t Row, size_t Col>
tuple<Matrix<Col>, BS<Row>, ll> gauss_elim(Matrix<Col> mat, BS<Row> b) {
    ll fcnt = 0;
    ll upper_row = 0;
    for (ll col = 0; col < Col; col++) {
        ll row = -1;
        for (ll r = upper_row; r < mat.size(); r++) {
            if (mat[r].test(col)) {
                row = r;
                break;
            }
        }
        if (row == -1) {
            fcnt++;
            continue;
        }
        swap(mat[upper_row], mat[row]);
        bool tmp = b[upper_row];
        b.set(upper_row, b[row]);
        b.set(row, tmp);
        for (ll r = 0; r < mat.size(); r++) {
            if (r == upper_row) continue;
            if (!mat[r].test(col)) continue;
            mat[r] ^= mat[upper_row];
            bool tmp = b[r] ^ b[upper_row];
            b.set(r, tmp);
        }
        upper_row++;
    }
    return make_tuple(mat, b, fcnt);
}

template <size_t Row, size_t Col>
tuple<Matrix<Col>, ll> gauss_elim(Matrix<Col> mat) {
    auto tmp = gauss_elim<Row, Col>(mat, BS<Row>(0));
    return make_tuple(get<0>(tmp), get<2>(tmp));
}
