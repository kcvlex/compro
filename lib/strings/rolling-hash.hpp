#pragma once
#include "util/template.hpp"
#include "util/types.hpp"
#include "math/modint.hpp"

namespace strings {

namespace internal {

ull get_base(ull lb, ull ub) {
    static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    // static std::mt19937_64 rng(0);
    return std::uniform_int_distribution<ull>(lb, ub)(rng);
}

template <ull Mod>
struct rolling_hash_builder {
    using mint = math::Modint<Mod>;
    using value_type = std::pair<mint, ull>;

    mint base;
    vec<mint> pows;

    rolling_hash_builder(const ull sz)
        : base(get_base(1000, Mod / 10)),
          pows(sz + 1)
    {
        pows[0] = 1;
        for (ull i = 1; i <= sz; i++) pows[i] = base * pows[i - 1];
    }

    value_type concat(const value_type &a, const value_type &b) const {
        const auto [ va, la ] = a;
        const auto [ vb, lb ] = b;
        return value_type((va * pows[lb] + vb), la + lb);
    }
};

template <ull Mod>
struct string_hash {
    using builder_type = rolling_hash_builder<Mod>;
    using value_type = typename builder_type::value_type;

    vec<value_type> data;
    builder_type *builder;

    template <typename Container>
    string_hash(const Container &s, builder_type *builder)
        : data(s.size() + 1), builder(builder)
    {
        data[0] = value_type(0, 0);
        for (ull i = 1; i <= s.size(); i++) {
            data[i].first = data[i - 1].first * builder->base + s[i - 1] % Mod;
            data[i].second = i;
        }
    }

    value_type substr(const ull l, const ull r) const {
        auto lv = data[l].first, rv = data[r].first;
        auto len = r - l;
        return value_type(rv - (lv * builder->pows[len]), len);
    }
};
    
template <ull Head, ull... Tail>
class multi_rolling_hash_type {
    using head_type = string_hash<Head>;
    using tail_type = multi_rolling_hash_type<Tail...>;
public:
    using hash_type = typename utility::tuple_append_front<head_type, typename tail_type::hash_type>::type;
    using builder_type = typename utility::tuple_append_front<typename head_type::builder_type,
                                                              typename tail_type::builder_type>::type;
    using value_type = typename utility::tuple_append_front<typename head_type::value_type,
                                                            typename tail_type::value_type>::type;
};

template <ull Mod>
class multi_rolling_hash_type<Mod> {
    using element = string_hash<Mod>;
public:
    using hash_type = std::tuple<element>;
    using builder_type = std::tuple<typename element::builder_type>;
    using value_type = std::tuple<typename element::value_type>;
};

}  // internal


// Usage
// Declare a variable RollingHash<mod1, ...> rh(size) at first.
// If you want to calculate string hash, call RollingHash<mod1, ...>::StringHash(str).
template <ull... Mods>
class RollingHash {
    using internal_type = internal::multi_rolling_hash_type<Mods...>;

public:
    using hash_type = typename internal_type::hash_type;
    using builder_type = typename internal_type::builder_type;
    using value_type = typename internal_type::value_type;
    using hash_ptr_type = typename utility::apply_tuple<std::add_pointer, hash_type>::type;
    using builder_ptr_type = typename utility::apply_tuple<std::add_pointer, builder_type>::type;
    constexpr static std::size_t mod_size = sizeof...(Mods);

    static builder_ptr_type** get_builders() {
        static builder_ptr_type *builders = nullptr;
        return &builders;
    }
   
private:
    template <std::size_t Index = 0>
    void construct(std::size_t sz) {
        if constexpr (mod_size == Index) {
        } else {
            using type = typename std::tuple_element<Index, builder_type>::type;
            std::get<Index>(**get_builders()) = new type(sz);
            construct<Index + 1>(sz);
        }
    }
    
    template <std::size_t Index = 0>
    void free() {
        if constexpr (mod_size == Index) {
        } else {
            delete std::get<Index>(**get_builders());
            free<Index + 1>();
        }
    }

    template <std::size_t Index = 0>
    void concat_rec(const value_type &a, const value_type &b, value_type &res) const {
        if constexpr (concat_rec == mod_size) {
        } else {
            std::get<Index>(res) = std::get<Index>(**get_builders())->concat(std::get<Index>(a), std::get<Index>(b));
            concat_rec<Index + 1>(a, b, res);
        }
    }

public:
    struct StringHash {
        hash_ptr_type hash;
        const std::size_t len;

        template <typename Container>
        StringHash(const Container &s) : len(s.size()) {
            construct<0, Container>(s);
        }

        ~StringHash() {
            this->free();
        }

        value_type substr(const ull l, const ull r) const {
            value_type ret;
            substr_rec(l, r, ret);
            return ret;
        }

        std::size_t size() const noexcept {
            return len;
        }

    private:
        template <std::size_t Index, typename Container>
        void construct(const Container &s) {
            if constexpr (Index == mod_size) {
            } else {
                using type = typename std::tuple_element<Index, hash_type>::type;
                std::get<Index>(hash) = new type(s, std::get<Index>(**get_builders()));
                construct<Index + 1, Container>(s);
            }
        }

        template <std::size_t Index = 0>
        void free() {
            if constexpr (Index == mod_size) {
            } else {
                delete std::get<Index>(hash);
                this->free<Index + 1>();
            }
        }

        template <std::size_t Index = 0>
        void substr_rec(const ull l, const ull r, value_type &res) const {
            if constexpr (Index == mod_size) {
            } else {
                std::get<Index>(res) = std::get<Index>(hash)->substr(l, r);
                substr_rec<Index + 1>(l, r, res);
            }
        }
    };

    RollingHash(const std::size_t sz) {
        assert(*get_builders() == nullptr);
        *get_builders() = new builder_ptr_type();
        construct(sz);
    }

    ~RollingHash() {
        free();
    }

    value_type concat(const value_type &a, const value_type &b) const {
        value_type ret;
        concat_rec(a, b, ret);
        return ret;
    }
};

}
