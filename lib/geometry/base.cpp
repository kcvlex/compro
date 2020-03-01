#include "../util/templage.cpp"

const double eps = 1e-6;
const long double pi = 3.14159265358979323846264338327950288419716939937510L;

using Point = std::complex<double>;

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

enum ccwd {
    Clock = -1,
    CoClock = 1,
    CAB = 2,
    ABC = -2,
    ACB = 0,
};

ccwd ccw(Point a, Point b, Point c) {
    b -= a;
    c -= a;
    double p, q;
    {
        auto tmp = std::conj(b) * c;
        p = tmp.real();
        q = tmp.imag();
    }
    if (0 < q) return CoClock;
    if (q < 0) return Clock;
    if (p < 0) return CAB;
    if (std::norm(b) < std::norm(c)) return ABC;
    return ACB;
}

bool is_zero(double a) {
    return std::abs(a) <= eps;
}

bool is_parallel(const Line &l1, const Line &l2) {
    return is_zero(cross(l1[1] - l1[0], l2[1] - l2[0]));
}

bool is_same_line(Point o, Point a, Point b) {
    return is_zero(cross(a - o, b - o));
}

bool intersect(const Line &p, const Line &q) {
    if (!is_parallel(p, q)) return true;
    return is_same_line(p[0], p[1], q[1]);
}

bool intersect(const Line &l, const Seg &s) {
    double a = cross(l[1] - l[0], s[0] - l[0]);
    double b = cross(l[1] - l[0], s[1] - l[0]);
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
    double l1 = std::abs(s[0] - s[1]);
    double l2 = std::abs(s[0] - p);
    double l3 = std::abs(s[1] - p);
    return is_zero(l2 + l3 - l1);
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

double distance(const Line &l, const Point &p) {
    return std::abs(p - proj(l, p));
}

double distance(const Line &p, const Line &q) {
    if (is_parallel(p, q)) return distance(p, q[0]);
    return 0;
}

double distance(const Line &l, const Seg &s) {
    if (intersect(l, s)) return 0;
    return std::min(distance(l, s[0]), distance(l, s[1]));
}

double distance(const Seg &s, const Point &p) {
    Line l(s[0], s[1]);
    Point pr = proj(l, p);
    if (intersect(s, pr)) return std::abs(pr - p);
    return std::min(std::abs(s[0] - p), std::abs(s[1] - p));
}

double distance(const Seg &p, const Seg &q) {
    if (intersect(p, q)) return 0;
    return var_min(distance(p, q[0]), distance(p, q[1]),
                   distance(q, p[0]), distance(q, p[1]));
}

double fix_arg(double arg) {
    arg += 2 * pi;
    if (2 * pi <= arg) arg -= 2 * pi;
    return arg;
}

bool comp_coclock(const Point &p1, const Point &p2) {
    return fix_arg(arg(p1)) < fix_arg(arg(p2));
}

Point rotate(const Point &p, double arg) {
    std::complex<double> r(std::cos(arg), std::sin(arg));
    return p * r;
}
