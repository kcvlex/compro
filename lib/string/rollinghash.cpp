#include "../util/template.cpp"

namespace str {

template <std::size_t Size>
struct RollingHash {
    const std::array<ll, Size> mods, bases;
    std::array<vec<ll>, Size> pows, hashs;

    template <ForwardIterator>
    RollingHash(ForwardIterator first, ForwardIterator last, std::array<ll, Size> mods, std::array<ll, Size> bases) {
        std::copy(ALL(mods), this->mods);
        std::copy(ALL(bases), this->bases);
        for (ll i = 0; i < Size; i++) {
            auto ite = first;
            ll mod = mods[i];
            ll base = bases[i];
            auto size = std::distance(first, last);
            vec<ll> hash(size + 1, 0);
            vec<ll> pow_(size + 1, 1);
            for (ll j = 0; j < size; j++) {
                hash[j + 1] = ((hash[j] * base) + *ite) % mod;
                pow_[j + 1] = pow_[j] * base % mod;
                std::advance(ite, 1);
            }
            pows[i] = std::move(pow_);
            hashs[i] = std::move(hash);
        }
    }

    // [l, r)
    template <ForwardIterator>
    void substr(ll l, ll r, ForwardIterator ite) {
        for (ll i = 0; i < mods.size(); i++) {
            ll h = hashs[i][r] - hashs[i][l] * pows[i][r - l] % mod + mod;
            if (mod <= h) h -= mod;
            *ite = h;
            ite++;
        }
    }

    template <template <typename> typename Container>
    void substr(ll l, ll r, Container<ll> &con) {
        substr(l, r, con.begin());
    }
};

}
