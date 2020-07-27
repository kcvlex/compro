#pragma once
#include "ccw.hpp"
#include "/home/taroy/kyopuro/lib/util/debug.hpp"

namespace geo {

vec<Point> build_convex_hull_incremental(vec<Point> pv) {
    std::sort(ALL(pv), comp_like_pair);
    auto minx = std::real(pv.front()),
         maxx = std::real(pv.back());

    {
        vec<Point> tmp;
        for (size_type i = 0; i < pv.size(); i++) {
            bool use = true;
            do {
                if (i == 0) break;
                if (i + 1 == pv.size()) break;
                auto pr = std::real(pv[i]);
                if (pr == minx) break;
                if (pr == maxx) break;
                if (pr != std::real(pv[i - 1])) break;
                if (pr != std::real(pv[i + 1])) break;
                use = false;
            } while (false);
            if (use) tmp.push_back(pv[i]);
        }
        size_type i = 0;
        while (i < tmp.size() && std::real(tmp[i]) == minx) i++;
        std::reverse(tmp.begin(), tmp.begin() + i);
        pv = std::move(tmp);
    }

    std::stack<Point, vec<Point>> ustk, lstk;
    size_type idx = 0;
    while (true) {
        auto p = pv[idx];
        bool f = (!ustk.empty() && std::real(p) != std::real(ustk.top()));
        ustk.push(pv[idx]);
        lstk.push(pv[idx]);
        idx++;
        if (f) break;
    }

    for (size_type i = idx; i < pv.size(); i++) {
        auto p = pv[i];
        while (2 <= ustk.size()) {
            auto p1 = ustk.top(); 
            ustk.pop();
            auto p2 = ustk.top();
            auto c = ccw(p, p1, p2);
            if (c == -1) continue;
            ustk.push(p1);
            break;
        }
        while (2 <= lstk.size()) {
            auto p1 = lstk.top();
            lstk.pop();
            auto p2 = lstk.top();
            auto c = ccw(p, p1, p2);
            if (c == 1) continue;
            lstk.push(p1);
            break;
        }
        ustk.push(p);
        lstk.push(p);
    }

    pv.clear();
    while (lstk.size()) {
        pv.push_back(lstk.top());
        DEBUG(lstk.top());
        lstk.pop();
    }
    std::reverse(ALL(pv));
    while (ustk.size()) {
        auto p = ustk.top();
        ustk.pop();
        if (std::real(p) == minx) continue;
        if (std::real(p) == maxx) continue;
        pv.push_back(p);
    }
    return pv;
}

}
