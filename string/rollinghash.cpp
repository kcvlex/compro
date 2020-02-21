#include "../util/template.cpp"

namespace str {

template <size_t Size>
struct RollingHash {
    const array<ll, Size> mods, bases;
    array<V<ll>, Size> pows, hashs;

    template <ForwardIterator>
    RollingHash(ForwardIterator first, ForwardIterator last, array<ll, Size> mods, array<ll, Size> bases) {
        copy(ALL(mods), this->mods);
        copy(ALL(bases), this->bases);
        for (ll i = 0; i < Size; i++) {
            auto ite = first;
            ll mod = mods[i];
            ll base = bases[i];
            auto size = distance(first, last);
            V<ll> hash(size + 1, 0);
            V<ll> pow_(size + 1, 1);
            for (ll j = 0; j < size; j++) {
                hash[j + 1] = ((hash[j] * base) + *ite) % mod;
                pow_[j + 1] = pow_[j] * base % mod;
                advance(ite, 1);
            }
            pows[i] = move(pow_);
            hashs[i] = move(hash);
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
};

}
