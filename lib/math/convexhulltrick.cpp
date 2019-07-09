#include <bits/stdc++.h>
using namespace std;
#define ALL(V) (V).begin(), (V).end()
#define ALLR(V) (V).rbegin(), (V).rend()
template <typename T> using V = vector<T>;
template <typename T> using VV = V<V<T>>;
template <typename T, typename U> using P = pair<T, U>;
using ll = int64_t;
using PLL = P<ll, ll>;

struct ConvexHullTrick {
    using Line = PLL;
    V<Line> lines;
    bool checked;

    ConvexHullTrick() : lines(0), checked(false) {}

    void update_query(Line l) {
        checked = false;
        lines.push_back(l);
    }

    void update_query(ll a, ll b) {
        update_query(Line(a, b));
    }

    bool is_necessary(Line a, Line b, Line c) {
        if(a < c) swap(a, c);
        ll v1 = (c.second - b.second) * (b.first - a.first);
        ll v2 = (b.second - a.second) * (c.first - b.first);
        return v1 < v2;
    }

    void check_lines() {
        checked = true;
        sort(ALLR(lines));
        auto tmp = lines;
        lines.clear();
        lines.push_back(tmp[0]);
        lines.push_back(tmp[1]);
        for(ll i = 2; i < tmp.size(); i++) {
            auto c = tmp[i];
            while(lines.size() >= 2) {
                auto a = *(lines.end() - 2);
                auto b = *(lines.end() - 1);
                if(!is_necessary(a, b, c)) {
                    lines.pop_back();
                } else {
                    break;
                }
            }
            lines.push_back(c);
        }
    }

    ll get_value(ll idx, ll x) {
        ll a, b;
        tie(a, b) = lines[idx];
        return a * x + b;
    }

    ll get_query(ll x) {
        if(!checked) {
            check_lines();
        }
        ll left_p = 0, right_p = lines.size();
        while(abs(right_p - left_p) > 2) {
            ll range = right_p - left_p;
            ll mid1 = left_p + range / 3;
            ll mid2 = left_p + 2 * range / 3;
            ll f1 = get_value(mid1, x);
            ll f2 = get_value(mid2, x);
            if(f1 < f2) {
                right_p = mid2;
            } else {
                left_p = mid1;
            }
        }
        return min(get_value(left_p, x), get_value(left_p + 1, x));
    }
};

// solution for https://atcoder.jp/contests/colopl2018-final-open/tasks/colopl2018_final_c
int main() {
    ll N;
    cin >> N;
    V<ll> A(N);
    for(ll &e : A) cin >> e;
    ConvexHullTrick CHT;
    for(ll j = 1; j <= N; j++) CHT.update_query(-2 * j, A[j - 1] + j * j);
    for(ll i = 1; i <= N; i++) cout << CHT.get_query(i) + i * i << endl;
    return 0;
}
