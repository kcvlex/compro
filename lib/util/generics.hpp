#pragma once
#include "template.cpp"

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

}
