#pragma once
#include "base.hpp"
#include "/home/taroy/kyopuro/lib/util/debug.hpp"

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

void ccw_sort(vec<Point> &v, Point o) {
    auto p = v[0];
    std::sort(v.begin() + 1, v.end(), 
              [&](Point a, Point b) {
                  if (a == b) return false;
                  int ca = ccw(o, p, a);
                  int cb = ccw(o, p, b);
                  if (ca != cb) return ca > cb;
                  return ccw(o, a, b) == 1;
              });
}

}
