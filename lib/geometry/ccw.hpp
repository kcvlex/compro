#pragma once
#include "base.cpp"

namespace geo {

/*      
 * Point c is
 *
 *         1
 * -----------------
 *  -2  a  0  b  2
 * -----------------
 *        -1
 */

int ccw(Point a, Point b, Point c) {
    b -= a;
    c -= a;
    value_type p, q;
    {
        auto tmp = std::conj(b) * c;
        p = tmp.real();
        q = tmp.imag();
    }
    if (0 < q) return 1;
    if (q < 0) return -1;
    if (p < 0) return 2;
    if (std::norm(b) < std::norm(c)) return -2;
    return 0;
}

}
