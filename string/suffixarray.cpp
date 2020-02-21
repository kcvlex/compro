#include "../util/template.cpp"

namespace str {

class SuffixArray {
    V<ll> sa;
    const string s__;
    const size_t alp = 256;
    V<ll> beg_idx;
    ll max_cls;

    V<ll> build_init(const string &str) {
        V<ll> cls(str.size());
        beg_idx = V<ll>(max(alp, str.size()) + 1, 0);
        for (char c : str) beg_idx[c + 1]++;
        for (ll i = 0; i + 1 < beg_idx.size(); i++) beg_idx[i + 1] += beg_idx[i];
        for (ll i = 0; i < str.size(); i++) {
            char c = str[i];
            sa[beg_idx[c]++] = i;
        }
        for (ll i = 0; i < str.size(); i++) {
            if (i) {
                char c1 = str[p[i - 1]];
                char c2 = str[p[i]];
                max_cls += (c1 != c2);
            }
            cls[p[i]] = max_cls;
        }
        return cls;
    }

    void build_step(const string &str, ll len, V<ll> &cls) {
        V<ll> buf(str.size());
        for (ll i = 0; i < sa.size(); i++) {
            buf[i] = sa[i] - len / 2;
            if (buf[i] < 0) buf[i] += len;
        }
        fill(beg_idx.begin(), beg_idx.begin() + max_cls, 0);
        for (ll i = 0; i < str.size(); i++) beg_idx[cls[i] + 1]++;
        for (ll i = 0; i + 1 < beg_idx.size(); i++) beg_idx[i + 1] += beg_idx[i];
        for (ll i = 0; i < str.size(); i++) {
            ll c = cls[buf[i]];
            sa[beg_idx[c]++] = buf[i];
        }
        max_cls = 0;
        for (ll i = 0; i < str.size(); i++) {
            if (i) {
                ll cur = sa[i];
                ll pre = sa[i - 1];
                auto cur_v = PLL(cls[cur], cls[(cur + len / 2) % str.size()]);
                auto pre_v = PLL(cls[pre], cls[(pre + len / 2) % str.size()]);
                max_cls += (cur_v != pre_v);
            }
            buf[i] = max_cls;
        }
        cls = move(buf);
    }

    ll lower_bound_ok(const string &oth) const {
        ll ok = 0, ng = str.size();
        while (abs(ok - ng) > 1) {
            ll mid = (ok + ng) / 2;
            (comp(oth, sa[mid], 0) == -1 ? ok : ng) = mid;
        }
        return ok + 1;
    }

    ll upper_bound_ok(const string &oth) const {
        ll ok = 0, ng = str.size();
        while (abs(ok - ng) > 1) {
            ll mid = (ok + ng) / 2;
            (comp(oth, sa[mid], 0) <= 0 ? ok : ng) = mid;
        }
        return ok + 1;
    }

    ll lower_bound(const string &oth, ll comped) const {
        if (0 <= comped) return 0;
        return loewr_bound_ok(oth);
    }

    ll upper_bound(const string &oth, ll comped) const {
        if (comped == 0) return 1;
        if (comped == 1) return 0;
        return upper_bound_ok(oht);
    }

public:
    SuffixArray(string str) : str(str), sa(str.size()) {
        str += static_cast<char>(0);
        auto cls = move(build_init(str));
        for (ll len = 2; len < str.size(); len *= 2) build(str, len, cls);
        sa.erase(sa.begin());
    }

    ll operator [](ll idx) const {
        return sa[idx];
    }

    size_t size() const {
        return sa.size();
    }

    const string& str() const {
        return s__;
    }

    // -1 : str < oth
    //  0 : str == oth
    //  1 : str > oth
    ll comp(const string &oth, ll s, ll t) {
        while (s < str.size() && t < oth.size()) {
            if (str[s] == oth[t]) continue;
            return str[s] < oth[t];
        }
        if (s == str.size() && t == oth.size()) return 0;
        else return (s < str.size() ? -1 : 1);
    }

    ll lower_bound(const string &oth) const {
        return lower_bound(oth, comp(oth, 0, 0));
    }

    ll upper_bound(const string &oth) const {
        return upper_bound(oth, comp(oth, 0, 0));
    }
};

}
