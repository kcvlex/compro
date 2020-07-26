#pragma once
#include "template.hpp"

namespace utility {

struct has_id_ele {
    template <typename T>
    auto operator ()(T &&t) -> decltype(T(), std::true_type()) { return std::true_type(); }
    std::false_type operator ()(...) { return std::false_type(); }
};

struct has_merge {
    template <typename T>
    auto operator ()(T &&t) -> decltype(T::merge(std::declval<T>(), std::declval<T>()), std::true_type()) { return std::true_type(); }
    std::false_type operator ()(...) { return std::false_type(); }
};

struct has_apply {
    template <typename M, typename Op>
    auto operator ()(M &&m, Op &&op) -> decltype(m.apply(op), std::true_type()) { return std::true_type(); }
    std::false_type operator ()(...) { return std::false_type(); }
};

template <typename F, typename... Args>
using callable = std::is_same<typename std::invoke_result<F, Args...>::type, std::true_type>;

template <typename M>
using is_monoid = std::conjunction<
    callable<has_id_ele, M>, callable<has_merge, M>>;

template <typename M, typename Op>
using enable_apply = callable<has_apply, M, Op>;

}
