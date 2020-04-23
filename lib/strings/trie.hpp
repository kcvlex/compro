#include "../util/template.hpp"
#include "../util/pool.hpp"

namespace strings {

template <typename T, std::size_t PoolSize, std::size_t LabelSize, typename Converter>
struct Trie {

    struct node {
        T dat;
        ll cnt = 0;
        std::array<ll, LabelSize> ch;

        node(T dat) : dat(dat) {
            std::fill(ALL(ch), -1);
        }

        node() : node(T()) { }
    };
    
    static Pool<node, PoolSize> pool;
    node root;
    const Converter &converter;

    Trie(const Converter &converter) : root(pool.alloc()), converter(converter)  { }

    template <template <typename> typename Container>
    const node& insert(const Container<T> &con, ssize_t idx, node &cur) {
        if (idx == con.size()) {
            cur.cnt++;
            return cur;
        }
        const auto &dat = con[idx];
        ll label = converter(dat);
        if (cur.ch[label] == -1) cur.ch[label] = pool.alloc(dat).second;
        auto &nxt = pool[cur.ch[label]];
        insert(con, idx + 1, nxt);
    }

    template <template <typename> typename Container>
    const node& insert(const Container<T> &con) {
        return insert(con, 0, root);
    }

};

}
