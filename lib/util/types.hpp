#pragma once
#include "template.hpp"

namespace utility {

template <typename Dummy> using is_false = std::false_type;

template <typename T, typename U>
struct tuple_append_front {
    static_assert(is_false<T>::value, "Error : This struct must not be refered");
};

template <template <typename> typename Type2Type, typename Head, typename... Tail>
struct apply_tuple_aux {
    using type = typename tuple_append_front<typename Type2Type<Head>::type,
                                             typename apply_tuple_aux<Type2Type, Tail...>::type>::type;
};

template <template <typename> typename Type2Type, typename T>
struct apply_tuple_aux<Type2Type, T> {
    using type = std::tuple<typename Type2Type<T>::type>;
};

template <typename T, typename... U>
struct tuple_append_front<T, std::tuple<U...>> {
    using type = std::tuple<T, U...>;
};

template <template <typename> typename Type2Type, typename T>
struct apply_tuple {
    static_assert(is_false<T>::value, "Error : This struct must not be refered");
};

template <template <typename> typename Type2Type, typename... Args>
struct apply_tuple<Type2Type, std::tuple<Args...>> {
    using type = typename apply_tuple_aux<Type2Type, Args...>::type;
};

}
