#pragma once
#include "grpah.hpp"

namespace graph {

struct RangeAddEdgeGraph : graph::Graph<true> {
    
    RangeAddEdgeGraph(size_type nsz) : Graph(), nsz(nsz), ceiled_nsz(ceil_pow2(nsz)) {
        this->resize(3 * ceiled_nsz);

        for (size_type i = 1; i < ceiled_nsz; i++) {
            this->add_edge(i, 2 * i + 0);
            this->add_edge(i, 2 * i + 1);
        }

        size_type offset = 2 * ceiled_nsz;
        auto get_idx = [&](size_type i) {
            if (i + offset < this->size()) return i + offset;
            return i;
        };
        for (size_type i = 1; i < ceiled_nsz; i++) {
            this->add_edge(get_idx(2 * i + 0), get_idx(i));
            this->add_edge(get_idx(2 * i + 1), get_idx(i));
        }
    }

    size_type nodes() const noexcept {
        return nsz;
    }

    size_type node2idx(size_type node) const noexcept {
        return node + ceiled_nsz;
    }

    size_type idx2node(size_type idx) const noexcept {
        return idx - ceiled_nsz;
    }
    
    void range_add_edge(size_type lsrc, size_type rsrc, 
                        size_type ldst, size_type rdst) 
    {
        range_add_edge(lsrc, rsrc, ldst, rdst, graph::Weight());
    }

    void range_add_edge(size_type lsrc, size_type rsrc, 
                        size_type ldst, size_type rdst, 
                        graph::Weight w) 
    {
        size_type src = this->size();
        size_type dst = src + 1;
        this->emplace_back();
        this->emplace_back();
        this->add_edge(src, dst, w);
        range_add_edge_aux(lsrc, rsrc, src, true);
        range_add_edge_aux(ldst, rdst, dst, false);
    }
   
private:
    const size_type nsz, ceiled_nsz;

    void range_add_edge_aux(size_type l, size_type r, size_type super, bool is_src) {
        size_type nl = l + ceiled_nsz, nr = r + ceiled_nsz;

        auto update = [&](size_type i) {
            if (is_src) this->add_edge(i, super);
            else this->add_edge(super, i);
        };

        bool init = true;

        auto offset = [&]() -> ll {
            if (init) return 0;
            if (!is_src) return 0;
            return 2 * ceiled_nsz;
        };

        while (nl < nr) {
            if (nl & 1) {
                update(nl + offset());
                nl++;
            }
            if (nr & 1) {
                nr--;
                update(nr + offset());
            }
            nl /= 2;
            nr /= 2;
            init = false;
        }
    }
};

}
