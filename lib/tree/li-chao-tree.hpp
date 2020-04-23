#include "../util/template.hpp"
#include "../util/ceil-pow2.hpp"

namespace tree {

template <typename T, typename Comp>
struct LiChaoTree {
    using line_type = std::pair<T, T>;

    Comp comp_b;
    T min_x, max_x, id_ele;
    line_type init;
    vec<line_type> nodes;
    vec<T> coor;
    ssize_t sz;

    LiChaoTree(const vec<T> &coor_arg, T min_x, T max_x, T id_ele, line_type init)
        : coor(coor_arg), min_x(min_x), max_x(max_x), init(init), id_ele(id_ele)
    {
        coor.push_back(min_x);
        coor.push_back(max_x);
        std::sort(ALL(coor));
        coor.erase(std::unique(ALL(coor)), coor.end());
        sz = ceil_pow2(coor.size());
        coor.resize(sz + 1, max_x + 1);
        nodes.resize(2 * sz - 1, init);
    }

    T comp(T a, T b) const noexcept {
        return comp_b(a, b) ? a : b;
    }

    T calc_y(line_type line, T x) const noexcept {
        if (line == init) return id_ele;
        ll a, b;
        std::tie(a, b) = line;
        return a * x + b;
    }

    T calc_y(ssize_t node, T x) const {
        return calc_y(nodes[node], x);
    }

    bool is_inited(ssize_t node) const {
        return nodes[node] != init;
    }

    void add_line(line_type line, ssize_t node, ssize_t lx_idx, ssize_t rx_idx) {
        if (nodes.size() <= node) return;
        if (!is_inited(node)) {
            nodes[node] = line;
            return;
        }

        ssize_t mx_idx = (lx_idx + rx_idx) / 2;
        T lx = coor[lx_idx], mx = coor[mx_idx], rx = coor[rx_idx];
        bool lcmp = comp_b(calc_y(line, lx), calc_y(node, lx));
        bool mcmp = comp_b(calc_y(line, mx), calc_y(node, mx));
        bool rcmp = comp_b(calc_y(line, rx), calc_y(node, rx));
        ssize_t lch = 2 * node + 1, rch = 2 * node + 2;

        if (!lcmp && !rcmp) return;
        if (lcmp && rcmp) {
            nodes[node] = line;
            return;
        }

        if (mcmp) {
            std::swap(line, nodes[node]);
            lcmp = !lcmp;
            rcmp = !rcmp;
        }

        if (lcmp) add_line(line, lch, lx_idx, mx_idx);
        if (rcmp) add_line(line, rch, mx_idx, rx_idx);
    }

    void add_line(line_type line) { 
        add_line(line, 0, 0, sz); 
    }

    void add_seg(line_type line, T qlx, T qrx) {
        ssize_t lx_idx = std::distance(coor.begin(), std::lower_bound(ALL(coor), qlx));
        ssize_t rx_idx = std::distance(coor.begin(), std::lower_bound(ALL(coor), qrx));
        ssize_t lnode = lx_idx + sz, rnode = rx_idx + sz;
        ssize_t cur_sz = 1;
        while (lnode < rnode) {
            if (lnode & 1) {
                add_line(line, lnode - 1, lx_idx, lx_idx + cur_sz);
                lnode++;
                lx_idx += cur_sz;
            }
            if (rnode & 1) {
                rnode--;
                rx_idx -= cur_sz;
                add_line(line, rnode - 1, rx_idx, rx_idx + cur_sz);
            }
            cur_sz *= 2;
            lnode /= 2;
            rnode /= 2;
        }
    }

    T query(T qx) const {
        T ret = id_ele;
        ssize_t lx_idx = 0, rx_idx = sz, node = 0;
        while (node < nodes.size()) {
            auto mx_idx = (lx_idx + rx_idx) / 2;
            T lx = coor[lx_idx], mx = coor[mx_idx], rx = coor[rx_idx];
            if (!(lx <= qx && qx < rx)) break;
            ret = comp(ret, calc_y(node, qx));
            if (lx <= qx && qx < mx) {
                rx_idx = mx_idx;
                node = 2 * node + 1;
            } else {
                lx_idx = mx_idx;
                node = 2 * node + 2;
            }
        }
        return ret;
    }
};

}
