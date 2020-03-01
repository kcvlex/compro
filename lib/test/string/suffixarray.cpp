#include "../../util/template.cpp"
#include "../../string/suffixarray.cpp"

int main() {
    string t;
    cin >> t;
    strings::SuffixArray sa(t);

    ll Q;
    cin >> Q;

    auto query = [&](const string &p) {
        ll l = sa.lower_bound(p);
        if (l == sa.size()) return false;
        ll si = sa[l], pi = 0;
        for (; si < t.size() && pi < p.size(); si++, pi++) if (t[si] != p[pi]) return false;
        return pi == p.size();
    };

    while (Q--) {
        string p;
        cin >> p;
        cout << (query(p) ? 1 : 0) << endl;
    }
    return 0;
}
