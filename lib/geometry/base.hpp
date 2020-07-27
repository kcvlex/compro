#pragma once
#include "../../lib/util/template.hpp"

namespace geo {

using value_type = double;
using Point = std::complex<value_type>;

const value_type eps = 1e-12;
const value_type pi = 3.14159265358979323846264338327950288419716939937510L;

const value_type err_v = std::numeric_limits<value_type>::max();
const Point err_p(err_v, err_v);

struct DualPoints : public std::pair<Point, Point> {
    using std::pair<Point, Point>::pair;

    Point& operator [](size_t idx) { return idx == 0 ? first : second; }
    
    const Point& operator [](size_t idx) const { return idx == 0 ? first : second; }
};

struct get_xy {
    value_type &x, &y;

    get_xy(value_type &x, value_type &y) : x(x), y(y) { }

    get_xy& operator =(const Point &p) {
        x = std::real(p);
        y = std::imag(p);
        return *this;
    }
};

struct Circle {
    Point p;
    value_type r;

    Circle(Point p, value_type r) : p(p), r(r) { }
};

struct Line : public DualPoints {
    using DualPoints::DualPoints;
};

struct Seg : public DualPoints {
    using DualPoints::DualPoints;
};

value_type dot(const Point &a, const Point &b) {
    return std::real(std::conj(a) * b);
}

value_type cross(const Point &a, const Point &b) {
    return std::imag(std::conj(a) * b);
}

bool is_zero(value_type a) {
    return std::abs(a) <= eps;
}

bool eq(value_type a, value_type b) {
    return is_zero(a - b);
}

// projection from p to l
Point proj(const Line &l, const Point &p) {
    value_type mul = dot(p - l[0], l[1] - l[0]) / norm(l[0] - l[1]);  // cos(theta) / (p-l[0]).norm()
    return l[0] + mul * (l[1] - l[0]);
}

Point refl(const Line &l, const Point &p) {
    Point pr = proj(l, p);
    return p + ((pr - p) * static_cast<value_type>(2));
}

value_type fix_arg(value_type arg) {
    while (arg < 0) arg += 2 * pi;
    while (2 * pi <= arg) arg -= 2 * pi;
    return arg;
}

value_type fix_arg(const Point &a) {
    return fix_arg(std::arg(a));
}

bool comp_coclock(const Point &p1, const Point &p2) {
    auto fa = fix_arg(arg(p1));
    auto fb = fix_arg(arg(p2));
    if (eq(fa, fb)) return std::norm(p1) < std::norm(p2);
    return fa < fb;
}

Point rotate(const Point &p, value_type arg) {
    std::complex<value_type> r(std::cos(arg), std::sin(arg));
    return p * r;
}

value_type cos(value_type a, value_type b, value_type c) {
    return (a * a + b * b - c * c) / (2 * a * b);
}

bool is_parallel(const Line &l1, const Line &l2) {
    return is_zero(cross(l1[1] - l1[0], l2[1] - l2[0]));
}

bool is_same_line(Point o, Point a, Point b) {
    return is_zero(cross(a - o, b - o));
}

}
