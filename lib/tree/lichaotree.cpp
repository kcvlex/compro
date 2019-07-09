#include <bits/stdc++.h>
/*
using namespace std;
using ll = int64_t;
using PLL = pair<ll, ll>;
template <typename T> using V = vector<T>;
*/

class LiChaoTree {
    using Line = PLL;
    V<ll> x_coors;
    V<Line> nodes;
    ll node_begin_idx;
    function<ll(ll, ll)> comp;    // maybe min or max
    function<bool(ll, ll)> comp_bool;
    Line invalid;
    ll inf, identity_ele;

    ll get_x_coor(ll xidx) { return xidx < x_coors.size() ? x_coors[xidx] : inf; }

    void add_line_(Line line, ll node_idx, ll lidx, ll ridx) {
        if(nodes.size() <= node_idx) return;
        if(nodes[node_idx] == invalid) {
            nodes[node_idx] = line;
            return;
        }

        ll midx = (lidx + ridx) / 2;
        ll lx = get_x_coor(lidx);
        ll mx = get_x_coor(midx);
        ll rx = get_x_coor(ridx);
        
        if(lx == inf) return;
        
        bool left_comp  = comp_bool(calc_y(line, lx), calc_y(nodes[node_idx], lx));
        bool mid_comp   = comp_bool(calc_y(line, mx), calc_y(nodes[node_idx], mx));
        bool right_comp = comp_bool(calc_y(line, rx), calc_y(nodes[node_idx], rx));

        if(left_comp && right_comp) {
            nodes[node_idx] = line;
            return;
        }
        if(!left_comp && !right_comp) return;
        if(mid_comp) {
            swap(line, nodes[node_idx]);
            left_comp = !left_comp;
            right_comp = !right_comp;
        }
        if(left_comp) add_line_(line, 2 * node_idx + 1, lidx, midx);
        else          add_line_(line, 2 * node_idx + 2, midx, ridx);
    }

public:

    LiChaoTree(const V<ll> &x_coors, function<ll(ll, ll)> comp, 
               ll identity_ele, ll inf, Line invalid = Line(1e9, 1e9))
        : comp(comp),
          comp_bool([this](ll a, ll b) { return this->comp(a, b) == a; }),
          identity_ele(identity_ele),
          inf(inf),
          invalid(invalid)
    {
        ll tmp = 1;
        while(tmp < x_coors.size()) tmp *= 2;
        node_begin_idx = tmp - 1;
        this->x_coors.resize(tmp);
        nodes.resize(2 * tmp - 1, invalid);
        for(ll i = 0; i < x_coors.size(); i++) this->x_coors[i] = x_coors[i];
        for(ll i = x_coors.size(); i < tmp; i++) this->x_coors[i] = inf;
    }

    ll calc_y(Line line, ll x) {
        if(line == invalid) return identity_ele;
        ll a, b;
        tie(a, b) = line;
        return a * x + b;
    }

    ll calc_y(ll idx, ll x) { return calc_y(nodes[idx], x); }
    
    void add_line(Line line) { add_line_(line, 0, 0, x_coors.size()); }
    void add_line(ll x, ll y) { add_line(Line(x, y)); }

    ll query(ll xidx) {
        ll node_idx = xidx + node_begin_idx;
        ll x = x_coors[xidx];
        ll ret = identity_ele;
        while(true) {
            if(nodes[node_idx] != invalid) {
                ll y = calc_y(node_idx, x);
                ret = comp(ret, y);
            }
            if(!node_idx) break;
            node_idx = (node_idx - 1) / 2;
        }
        return ret;
    }
};
