#pragma once
#include "util/template.hpp"

namespace strings {

struct AutomatonNode {
    using node_ptr = AutomatonNode*;
    vec<node_ptr> nexts;
    vec<size_type> accept;
    node_ptr fail;

    AutomatonNode(size_type sz) : nexts(sz + 1, nullptr)
    {
    }
};

struct AhoCorasick {
    using node_ptr = AutomatonNode<T>::node_ptr;
    const std::string &kinds;
    node_ptr root;

    AhoCorasick(std::string kinds_arg) 
        : kinds(std::move(kinds_arg)), 
          root(new AutomatonNode(kinds.size()))
    {
        root->fail = root;
    }

    void build(const vec<std::string> &vs) {
        for (size_type i = 0; i < vs.size(); i++) {
            const auto &s = vs[i];
            auto cur = root;
            for (size_type j = 0; j < kinds.size(); j++) {
                char c = kinds[j];
                if (!cur->nexts[j]) cur->nexts[j] = new AutomatonNode(kinds.size());
                cur = cur->nexts[j];
            }
            cur->accept.push_back(i);
        }

        std::queue<node_ptr> que;
        for (size_type i = 0; i < kinds.size(); i++) {
            auto tmp = root->nexts[i];
            if (tmp) {
                tmp->fail = root;
                que.push_back(tmp);
            } else {
                root->nexts[i] = root;
            }
        }

        while (que.size()) {
            auto cur = que.front();
            que.pop();
            for (size_type i = 0; i < kinds.size(); i++) {
                auto nxt = cur->nexts[i];
                if (!nxt) continue;
                auto f = cur->fail;
                while (!f->nexts[i]) f = f->fail;
                nxt->fail = f->nexts[i];
                que.push(nxt);
            }
        }
    }

    // TODO
    size_type get_idx(char c) {
        return c - 'a';
    }

    vec<size_type> match(const std::string &s) {
        auto cur = root;
        vec<size_type> ret;
        for (char c : s) {
            auto idx = get_idx(c);
            while (!cur->nexts[idx]) cur = cur->fail;
            cur = cur->nexts[idx];
            const auto &accept = cur->accept;
            auto sz = ret.size();
            ret.resize(sz + accept.size());
            std::copy(ALL(accept), ret.begin() + sz);
        }
        return ret;
    }
};

}
