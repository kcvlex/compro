#include "../../lib/tree/splay-tree.cpp"

// solution for https://atcoder.jp/contests/arc033/tasks/arc033_3
int main() {
    tree::SplayTree<int> stree;
    int n;
    std::cin >> n;
    while (n--) {
        int a, b;
        std::cin >> a >> b;
        if (a == 1) {
            stree.insert(b);
        } else {
            auto ptr = stree[b - 1];
            std::cout << ptr->key << '\n';
            stree.erase(ptr);
        }
    }
    return 0;
}
