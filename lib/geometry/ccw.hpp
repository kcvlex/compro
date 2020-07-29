#pragma once
#include "base.hpp"

namespace geo {

/*      
 * Point c is
 *
 *         +1
 * ------------------
 *   +2  a    b  -2
 * ------------------
 *         -1
 */

int ccw(Point a, Point b, Point c) {
    b -= a; c -= a;
    if (cross(b, c) > 0)             return +1;
    if (cross(b, c) < 0)             return -1;
    if (dot(b, c) < 0)               return +2;
    if (std::norm(b) < std::norm(c)) return -2;
    return 0;
}

// WARNING : v has o
void ccw_sort(vec<Point> &v, Point o) {
    int lis[] = { -2, 1, 2, -1, };
    auto get_ccw_idx = [&](int c) {
        for (int i = 0; i < 4; i++) if (c == lis[i]) return i;
        return 4;
    };

    auto p = v[0];
    std::sort(v.begin() + 1, v.end(), 
              [&](Point a, Point b) {
                  if (a == b) return false;
                  int ca = ccw(o, p, a);
                  int cb = ccw(o, p, b);
                  if (ca != cb) return get_ccw_idx(ca) < get_ccw_idx(cb);
                  auto res = ccw(o, a, b);
                  if (std::abs(res) == 1) return res == 1;
                  return comp_like_pair(a, b);
              });
}

}
