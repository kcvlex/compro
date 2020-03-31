#pragma once
#include "template.cpp"

namespace utility {

template <ssize_t Size, typename T, template <T...> typename Class, const T *Array, T ...Args>
struct pass_array_helper {
    using type = typename pass_array_helper<Size - 1, T, Class, Array, Array[Size - 1], Args...>::type;
};

template <typename T, template <T...> typename Class, const T *Array, T ...Args>
struct pass_array_helper<0, T, Class, Array, Args...> {
    using type = Class<Args...>;
};

template <typename T, template <T...> typename Class, const T *Array>
struct pass_array {
    using type = typename pass_array_helper<sizeof(Array) / sizeof(T), T, Class, Array>::type;
};

}
