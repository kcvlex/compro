#pragma once
#include "../../lib/util/template.cpp"

namespace geo {

const double eps = 1e-6;
const long double pi = 3.14159265358979323846264338327950288419716939937510L;

using Point = std::complex<double>;
const double err_v = std::numeric_limits<double>::max();
const Point err_p(err_v, err_v);

struct DualPoints : public std::pair<Point, Point> {
    using std::pair<Point, Point>::pair;

    Point& operator [](size_t idx) { return idx == 0 ? first : second; }
    
    const Point& operator [](size_t idx) const { return idx == 0 ? first : second; }
};

struct get_xy {
    double &x, &y;

    get_xy(double &x, double &y) : x(x), y(y) { }

    get_xy& operator =(const Point &p) {
        x = std::real(p);
        y = std::imag(p);
        return *this;
    }
};

struct Circle {
    Point p;
    double r;

    Circle(Point p, double r) : p(p), r(r) { }
};

struct Line : public DualPoints {
    using DualPoints::DualPoints;
};

struct Seg : public DualPoints {
    using DualPoints::DualPoints;
};

double dot(const Point &a, const Point &b) {
    return (std::conj(a) * b).real();
}

double cross(const Point &a, const Point &b) {
    return (std::conj(a) * b).imag();
}

bool is_zero(double a) {
    return std::abs(a) <= eps;
}

bool eq(double a, double b) {
    return is_zero(a - b);
}

// projection from p to l
Point proj(const Line &l, const Point &p) {
    double mul = dot(p - l[0], l[1] - l[0]) / norm(l[0] - l[1]);  // cos(theta) / (p-l[0]).norm()
    return l[0] + mul * (l[1] - l[0]);
}

Point refl(const Line &l, const Point &p) {
    Point pr = proj(l, p);
    return p + (2. * (pr - p));
}

double fix_arg(double arg) {
    arg += 2 * pi;
    if (2 * pi <= arg) arg -= 2 * pi;
    return arg;
}

double fix_arg(const Point &a) {
    return fix_arg(std::arg(a));
}

bool comp_coclock(const Point &p1, const Point &p2) {
    return fix_arg(arg(p1)) < fix_arg(arg(p2));
}

Point rotate(const Point &p, double arg) {
    std::complex<double> r(std::cos(arg), std::sin(arg));
    return p * r;
}

double cos(double a, double b, double c) {
    return (a * a + b * b - c * c) / (2 * a * b);
}

bool is_parallel(const Line &l1, const Line &l2) {
    return is_zero(cross(l1[1] - l1[0], l2[1] - l2[0]));
}

bool is_same_line(Point o, Point a, Point b) {
    return is_zero(cross(a - o, b - o));
}

}
