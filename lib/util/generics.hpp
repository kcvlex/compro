#pragma once
#include "template.hpp"

namespace utility {

template <typename T, T... Args>
constexpr std::array<T, sizeof...(Args)> 
make_array(std::integer_sequence<T, Args...>) {
    return {{ Args... }};
}

template <typename, typename>
struct tuple_concat;

template <typename... Args1, typename... Args2>
struct tuple_concat<std::tuple<Args1...>, std::tuple<Args2...>> {
    using type = std::tuple<Args1..., Args2...>;
};

template <ll... N1, ll... N2>
constexpr auto concat_integer_sequence(std::integer_sequence<ll, N1...>, std::integer_sequence<ll, N2...>) {
    return std::integer_sequence<ll, N1..., N2...>();
}

template <ll Head, ll... Tail>
struct reverse_sequence {
    using head_type = std::integer_sequence<ll, Head>;
    using tail_type = typename reverse_sequence<Tail...>::type;
    using type = decltype(concat_integer_sequence(std::declval<tail_type>(), std::declval<head_type>()));
};

template <ll Head>
struct reverse_sequence<Head> {
    using type = std::integer_sequence<ll, Head>;
};

template <ll... N>
constexpr auto reverse_ns(std::integer_sequence<ll, N...>) {
    return typename reverse_sequence<N...>::type();
}

}
