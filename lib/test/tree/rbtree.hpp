#include "../../lib/tree/rbtree.cpp"

int main() {
    ll n;
    std::cin >> n;
    tree::RedBlackTree<ll> rbtree;
    while (n--) {
        ll t;
        std::cin >> t;
        if (t == 1) {
            ll x;
            std::cin >> x;
            rbtree.insert(x);
        } else {
            ll x;
            std::cin >> x;
            std::cout << rbtree.erase(x - 1)->dat << '\n';
        }
    }
    return 0;
}
