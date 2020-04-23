#pragma once
#include "base.hpp"
#include "ccw.hpp"

namespace geo {

bool intersect(const Line &p, const Line &q) {
    if (!is_parallel(p, q)) return true;
    return is_same_line(p[0], p[1], q[1]);
}

bool intersect(const Line &l, const Seg &s) {
    value_type a = cross(l[1] - l[0], s[0] - l[0]);
    value_type b = cross(l[1] - l[0], s[1] - l[0]);
    return a * b < 0;
}

bool intersect(const Seg &p, const Seg &q) {
    bool pf = (ccw(p[0], p[1], q[0]) * ccw(p[0], p[1], q[1])) <= 0;
    bool qf = (ccw(q[0], q[1], p[0]) * ccw(q[0], q[1], p[1])) <= 0;
    return pf && qf;
}

bool intersect(const Line &l, const Point &p) {
    return is_same_line(p, l[0], l[1]);
}

bool intersect(const Seg &s, const Point &p) {
    value_type l1 = std::abs(s[0] - s[1]);
    value_type l2 = std::abs(s[0] - p);
    value_type l3 = std::abs(s[1] - p);
    return is_zero(l2 + l3 - l1);
}

value_type distance(const Point &p1, const Point &p2) {
    return std::abs(p1 - p2);
}

value_type distance(const Line &l, const Point &p) {
    return std::abs(p - proj(l, p));
}

value_type distance(const Line &p, const Line &q) {
    if (is_parallel(p, q)) return distance(p, q[0]);
    return 0;
}

value_type distance(const Line &l, const Seg &s) {
    if (intersect(l, s)) return 0;
    return std::min(distance(l, s[0]), distance(l, s[1]));
}

value_type distance(const Seg &s, const Point &p) {
    Line l(s[0], s[1]);
    Point pr = proj(l, p);
    if (intersect(s, pr)) return std::abs(pr - p);
    return std::min(std::abs(s[0] - p), std::abs(s[1] - p));
}

value_type distance(const Seg &p, const Seg &q) {
    if (intersect(p, q)) return 0;
    return var_min(distance(p, q[0]), distance(p, q[1]),
                   distance(q, p[0]), distance(q, p[1]));
}

int intersect(const Circle &c1, const Circle &c2) {
    if (c1.r < c2.r) return intersect(c2, c1);
    value_type d = distance(c1.p, c2.p);
    if (d + c2.r < c1.r) return 1;
    if (eq(d + c2.r, c1.r)) return 2;
    if (d < c1.r + c2.r) return 3;
    if (eq(c1.r - c2.r, d)) return 4;
    return 0;
}

std::pair<Point, Point> crosspoint(const Circle &c1, const Circle &c2) {
    value_type d = distance(c1.p, c2.p);
    value_type rc = cos(d, c1.r, c2.r) * c1.r;
    value_type rs = std::sqrt(c1.r * c1.r - rc * rc);
    auto unit_v = (c2.p - c1.p) / d;
    return std::make_pair(c1.p + unit_v * Point(rc, rs),
                          c1.p + unit_v * Point(rc, -rs));
}

}
