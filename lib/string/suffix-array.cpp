#include "../util/template.cpp"

namespace strings {

class SuffixArray {
    vec<ll> sa, beg_idx;
    std::string s__;
    const std::size_t alp = 256;

    vec<ll> build_init() {
        vec<ll> cls(sa.size());
        beg_idx = vec<ll>(std::max<ll>(sa.size(), alp) + 2, 0);
        for (char c : s__) beg_idx[c + 1]++;
        for (ll i = 0; i + 1 < beg_idx.size(); i++) beg_idx[i + 1] += beg_idx[i];
        for (ll i = 0; i < size(); i++) {
            char c = s__[i];
            sa[beg_idx[c]++] = i;
        }
        
        ll max_cls = 0;
        for (ll i = 0; i < size(); i++) {
            if (i) {
                char c1 = s__[sa[i - 1]];
                char c2 = s__[sa[i]];
                max_cls += (c1 != c2);
            }
            cls[sa[i]] = max_cls;
        }
        return cls;
    }

    void build_step(ll len, vec<ll> &cls) {
        vec<ll> buf(cls.size());
        for (ll i = 0; i < sa.size(); i++) {
            auto idx = sa[i] - len / 2;
            if (idx < 0) idx = sa.size() + idx;
            buf[i] = idx;
        }
        std::fill(ALL(beg_idx), 0);
        for (ll i = 0; i < size(); i++) beg_idx[cls[i] + 1]++;
        for (ll i = 0; i + 1 < beg_idx.size(); i++) beg_idx[i + 1] += beg_idx[i];
        for (ll i = 0; i < size(); i++) {
            ll b = buf[i];
            ll c = cls[b];
            sa[beg_idx[c]++] = b;
        }

        auto get_cls = [&](ll idx) {
            ll fst = cls[idx];
            ll snd_idx = idx + len / 2;
            return std::make_pair(fst, cls[snd_idx % sa.size()]);
        };

        ll max_cls = 0;
        for (ll i = 0; i < size(); i++) {
            if (i) {
                ll cur = sa[i];
                ll pre = sa[i - 1];
                max_cls += get_cls(cur) != get_cls(pre);
            }
            buf[sa[i]] = max_cls;
        }
        cls = std::move(buf);
    }

    ll lower_bound_ok(const std::string &oth) const {
        ll ok = 0, ng = size();
        while (abs(ok - ng) > 1) {
            ll mid = (ok + ng) / 2;
            (comp(oth, sa[mid], 0) == -1 ? ok : ng) = mid;
        }
        return ok + 1;
    }

    ll upper_bound_ok(const std::string &oth) const {
        ll ok = 0, ng = size();
        while (abs(ok - ng) > 1) {
            ll mid = (ok + ng) / 2;
            (comp(oth, sa[mid], 0) <= 0 ? ok : ng) = mid;
        }
        return ok + 1;
    }

    ll lower_bound(const std::string &oth, ll comped) const {
        if (0 <= comped) return 0;
        return lower_bound_ok(oth);
    }

    ll upper_bound(const std::string &oth, ll comped) const {
        if (comped == 0) return 1;
        if (comped == 1) return 0;
        return upper_bound_ok(oth);
    }

    ll ceil_pow2(ll n) {
        ll ret = 1;
        while (ret < n) ret *= 2;
        return ret;
    }

public:
    SuffixArray(std::string str) : s__(str) {
        auto tmp = s__;
        s__ += static_cast<char>(0);
        ll pow2 = ceil_pow2(s__.size());
        ll diff = pow2 - s__.size();
        s__ += std::string(diff, 0); 
        sa.resize(pow2);
        auto cls = std::move(build_init());
        for (ll len = 2; len < size(); len *= 2) build_step(len, cls);
        std::reverse(ALL(sa));
        sa.erase(sa.end() - (pow2 - tmp.size()), sa.end());
        std::reverse(ALL(sa));
        s__ = std::move(tmp);
    }

    ll operator [](ll idx) const {
        return sa[idx];
    }

    size_t size() const {
        return sa.size();
    }

    const std::string& str() const {
        return s__;
    }

    // -1 : str < oth
    //  0 : str == oth
    //  1 : str > oth
    ll comp(const std::string &oth, ll s, ll t) const {
        for (; s < size() && t < oth.size(); s++, t++) {
            if (s__[s] == oth[t]) continue;
            return (s__[s] < oth[t] ? -1 : 1);
        }
        if (s == size() && t == oth.size()) return 0;
        else return (s < size() ? 1 : -1);
    }

    ll lower_bound(const std::string &oth) const {
        return lower_bound(oth, comp(oth, sa[0], 0));
    }

    ll upper_bound(const std::string &oth) const {
        return upper_bound(oth, comp(oth, sa[0], 0));
    }
};

}
