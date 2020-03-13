#include "../../lib/tree/splaytree.cpp"

using namespace std;
const ll inf = 5e15;

// solution for https://atcoder.jp/contests/arc033/tasks/arc033_3
int main() {
    ll Q;
    cin >> Q;
    tree::SplayTree<std::string> st;
    while (Q--) {
        ll t;
        std::cin >> t;
        if (t == 1) {
            std::string x;
            std::cin >> x;
            st.insert(x);
        } else {
            ll x;
            std::cin >> x;
            cout << st.query(x) << endl;
        }
    }
    return 0;
}

