#include "../../util/template.hpp"
#include "../../string/suffixarray.hpp"

int main() {
    std::string t;
    std::cin >> t;
    strings::SuffixArray sa(t);

    ll q;
    std::cin >> q;

    auto query = [&](const std::string &p) {
        ll l = sa.lower_bound(p);
        if (l == sa.size()) return false;
        ll si = sa[l], pi = 0;
        for (; si < t.size() && pi < p.size(); si++, pi++) if (t[si] != p[pi]) return false;
        return pi == p.size();
    };

    while (q--) {
        std::string p;
        std::cin >> p;
        std::cout << (query(p) ? 1 : 0) << '\n';
    }
    return 0;
}
