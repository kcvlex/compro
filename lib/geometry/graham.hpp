#pragma once
#include "ccw.hpp"

namespace geo {

Point get_one(const vec<Point> &pv) {
    auto p1 = pv[1] - pv[0];
    size_type i = 2;
    for (; i < pv.size(); i++) {
        auto p2 = pv[i] - pv[0];
        auto v = std::abs(dot(p1, p2));
        if (!eq(v * v, std::norm(p1) * std::norm(p2))) break;
    }
    auto p = pv[0] + pv[1] + pv[i];
    return Point(std::real(p) / 3, std::imag(p) / 3);
}

vec<Point> graham(vec<Point> pv) {
    size_type n = pv.size();
    auto cp = get_one(pv);
    ccw_sort(pv, cp);
    auto get_p = [&](size_type i) { return pv[i % n]; };
    auto ite = std::max_element(ALL(pv), [](Point a, Point b) { return std::real(a) < std::real(b); });
    size_type idx = std::distance(pv.begin(), ite);
    vec<Point> ret = { get_p(idx), get_p(idx + 1) };

    auto on_line = [&](Point a, Point b, Point c) {
        b -= a; c -= a;
        auto v = dot(b, c);
        return eq(v * v, std::norm(b) * std::norm(c));
    };

    auto check = [&](Point p) {
        // a, b, p is convex or not
        size_type tmp = ret.size();
        Point a = ret[tmp - 2],
              b = ret[tmp - 1];
        if (on_line(a, b, p)) return true;
        a -= cp; b -= cp; p -= cp;
        int c = ccw(a, b, p);
        return c != -1;
    };
    for (size_type i = 2; i <= n; i++) {
        auto p = get_p(idx + i);
        while (1 < ret.size() && !check(p)) ret.pop_back();
        ret.push_back(p);
    }
    if (ret.front() == ret.back()) ret.pop_back();
    return ret;
}

}
