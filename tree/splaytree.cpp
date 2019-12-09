#include "../template.cpp"

template <typename Key> struct SplayTreeNode {
    using node_ptr = SplayTreeNode<Key> *;
    Key key;
    node_ptr par, l, r;
    size_t size;

    SplayTreeNode(Key key, node_ptr par)
        : key(key), par(par), l(nullptr), r(nullptr), size(1) {}
    SplayTreeNode(Key key) : SplayTreeNode(key, nullptr) {}
    SplayTreeNode() : SplayTreeNode(Key()) {}

    bool is_l() const { return par != nullptr && par->l == this; }

    bool is_root() const { return par == nullptr; }

    void rotate(bool l_ch) {
        if(is_root()) return;
        auto p = par;
        auto pp = p->par;
        par = pp;
        if(pp != nullptr) (p->is_l() ? pp->l : pp->r) = this;
        if(l_ch) {
            p->l = r;
            if(r != nullptr) r->par = p;
            r = p;
            p->par = this;
        } else {
            p->r = l;
            if(l != nullptr) l->par = p;
            l = p;
            p->par = this;
        }
        p->update_size();
        update_size();
    }

    void update_size() {
        size = 1;
        if(l != nullptr) size += l->size;
        if(r != nullptr) size += r->size;
    }

    void rotate() { rotate(is_l()); }

    void zig() { rotate(); }

    void zig_zig() {
        par->rotate();
        rotate();
    }

    void zig_zag() {
        rotate();
        rotate();
    }

    void splay() {
        while(!is_root()) {
            if(par->is_root()) rotate();
            else if(is_l() ^ par->is_l()) zig_zag();
            else zig_zig();
        }
    }

    node_ptr set_l(node_ptr nl) {
        l = nl;
        if(nl) nl->par = this;
        update_size();
        return this;
    }

    node_ptr set_r(node_ptr nr) {
        r = nr;
        if(nr) nr->par = this;
        update_size();
        return this;
    }

    node_ptr cut_l() {
        auto ret = l;
        if(l) l->par = nullptr;
        l = nullptr;
        update_size();
        return ret;
    }

    node_ptr cut_r() {
        auto ret = r;
        if(r) r->par = nullptr;
        r = nullptr;
        update_size();
        return ret;
    }
};

template <typename STree> STree merge(STree t1, STree t2);
template <typename STree, typename Key> pair<STree, STree> split(STree tree, Key k);

template <typename Key, typename Comp = less<Key>> struct SplayTree {
    using Node = SplayTreeNode<Key>;
    using Tree = SplayTree<Key, Comp>;
    using node_ptr = typename SplayTreeNode<Key>::node_ptr;
    node_ptr root;

    SplayTree(node_ptr root) : root(root) {};
    SplayTree() : SplayTree(nullptr) {}

    bool comp(Key x, Key y) { return Comp()(x, y); }

    node_ptr find_max() {
        node_ptr cur = root;
        while(cur && cur->r) cur = cur->r;
        splay(cur);
        return cur;
    }

    // TODO: ifk isn't found
    node_ptr find(Key k) {
        auto cur = root;
        while(cur) {
            if(cur->key == k) break;
            if(comp(k, cur->key)) {
                if(cur->l) cur = cur->l;
                else break;
            } else if(comp(cur->key, k)) {
                if(cur->r) cur = cur->r;
                else break;
            }
        }
        if(cur) splay(cur);
        return cur;
    }

    void insert(Key key) {
        if(root == nullptr) {
            root = new Node(key);
            return;
        }
        Tree lt, rt;
        tie(lt, rt) = split(*this, key);
        root = new Node(key);
        root->set_l(lt.root);
        root->set_r(rt.root);
    }

    void erase(Key x) { 
        auto nx = this->find(x);
        if(nx == nullptr || nx->key != x) return;
        Tree lt, rt;
        tie(lt, rt) = split(*this, nx->key);

        {
            node_ptr tmp = nullptr;
            if(lt.root && lt.root->key == x) {
                tmp = lt.root;
                lt.root = lt.root->cut_l();
            } else if(rt.root && rt.root->key == x) {
                tmp = rt.root;
                rt.root = rt.root->cut_r();
            }
            if(tmp) delete tmp;
        }

        if(lt.root && rt.root) {
            lt.find_max();
            lt.root->set_r(rt.root);
            root = lt.root;
        } else if(lt.root) {
            root = lt.root;
        } else if(rt.root) {
            root = rt.root;
        } else {
            root = nullptr;
        }
    }

    ll query(size_t ord) {
        size_t cnt = ord;
        node_ptr cur = root;
        ll ret = 0;
        while(cnt) {
            auto l_size = (cur->l == nullptr ? 0 : cur->l->size);
            if(l_size + 1 == cnt) {
                ret = cur->key;
                break;
            }
            if(l_size + 1 < cnt) {
                cnt -= l_size + 1;
                cur = cur->r;
            } else {
                cur = cur->l;
            }
        }
        erase(cur->key);
        return ret;
    }

    void splay(node_ptr node) {
        node->splay();
        root = node;
    }
};

template <typename STree>
STree merge(STree t1, STree t2) {
    if(!t1.root) return t2;
    if(!t2.root) return t1;
    auto t1_max = t1.find_max();
    t1.splay(t1_max);
    t1.root->set_r(t2.root);
    return t1;
}

template <typename STree, typename Key>
pair<STree, STree> split(STree tree, Key k) {
    tree.find(k);
    auto root = tree.root;
    auto nl = root->cut_l();
    auto nr = root->cut_r();
    if(tree.comp(k, root->key)) nr = root->set_r(nr);
    else nl = root->set_l(nl);
    return make_pair(STree(nl), STree(nr));
}

const ll inf = 5e15;

// solution for https://atcoder.jp/contests/arc033/tasks/arc033_3
int main() {
    ll Q;
    cin >> Q;
    SplayTree<ll> st;
    while(Q--) {
        ll t, x;
        cin >> t >> x;
        //st.dump();
        if(t == 1) st.insert(x);
        else cout << st.query(x) << endl;
    }
    return 0;
}

