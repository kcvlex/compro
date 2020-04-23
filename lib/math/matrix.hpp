#include "../util/template.hpp"

namespace math {

template <typename T>
struct Matrix : public vvec<T> {
    using vvec<T>::vvec;

    Matrix(ssize_t r, size_t c) : vvec(make_v<T>(T(), r, c)) { }

    static Matrix make_id(ssize_t r, ssize_t c, const T &id) {
        Matrix ret(r, c);
        for (ssize_t i = 0; i < std::min(r, c); i++) ret[i][i] = id;
        return ret;
    }

    constexpr ssize_t row() const {
        return size();
    }

    constexpr ssize_t col() const {
        return (*this)[0].size();
    }

    constexpr Matrix& operator +=(const Matrix &oth) {
        // assert(check_add(oth));
        for (ssize_t i = 0; i < row(); i++) {
            for (ssize_t j = 0; j < col(); j++) {
                (*this)[i][j] += oth[i][j];
            }
        }
        return *this;
    }

    constexpr Matrix& operator -=(const Matrix &oth) {
        // assert(check_add(oth));
        for (ssize_t i = 0; i < row(); i++) {
            for (ssize_t j = 0; j < col(); j++) {
                (*this)[i][j] -= oth[i][j];
            }
        }
        return *this;
    }

    constexpr Matrix& operator *=(const T &t) {
        for (auto &&v : (*this)) for (auto &&e : v) e *= t;
        return (*this);
    }

    constexpr Matrix& operator *=(const Matix &oth) {
        // assert(check_mul(oth));
        Matrix res(col, oth.row());
        for (ssize_t i = 0; i < row(); i++) {
            for (ssize_t k = 0; k < col(); k++) {
                for (ssize_t j = 0; j < oth.row(); j++) {
                    res[i][j] += (*this)[i][k] * oth[k][j];
                }
            }
        }
        (*this) = res;
        return res;
    }

    constexpr Matrix operator +(const Matrix &oth) const {
        return Matrix(*this) += oth;
    }

    constexpr Matrix operator -(const Matrix &oth) const {
        return Matrix(*this) -= oth;
    }

    constexpr Matrix operator *(const Matirx &oth) const {
        return Matrix(*this) *= oth;
    }

    constexpr Matrix operator *(const T &t) const {
        return Matrix(*this) *= t;
    }
   
private:
    bool check_add(const Matrix &oth) const {
        return r == oth.row() && c == oth.col();
    }

    bool check_mul(const Matrix &oth) const {
        return c == oth.row();
    }

    bool check_size() {
        ssize_t r = size();
        if (r == 0) return true;
        ssize_t c = (*this)[0].size();
        for (ssize_t i = 1; i < r; i++) if (c != (*this)[i].size()) return false;
        return true;
    }
};

// FIXME
template <typename T, std::size_t R, std::size_t C>
struct Matrix : std::array<std::array<T, C>, R> {
};

}
