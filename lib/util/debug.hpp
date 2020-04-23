#pragma once

#define DEBUG_MODE
#ifdef DEBUG_MODE
#define DEBUG(...) debug_f(std::string(#__VA_ARGS__), __VA_ARGS__)
#define DEBUG_ENDL std::endl<<std::flush
#define DEBUG_SEPARATOR_LINE std::cout<<"=================\n"
#else
#define DEBUG(...) 0
#define DEBUG_ENDL 0
#define DEBUG_SEPARATOR_LINE 0
#endif
#define DEBUG_ENDL_S(S) ((S).size()?"\n":"")<<std::flush;

namespace debug {

namespace {

struct begin_func__ {
    template <typename T>
    auto operator ()(T t) -> decltype(std::begin(std::declval<T>()), std::true_type{}) { return std::true_type{}; }
    std::false_type operator ()(...) { return std::false_type{}; }
};

struct to_string_func__ {
    template <typename T>
    auto operator ()(T t) -> decltype(std::to_string(std::declval<T>()), std::true_type{}) { return std::true_type{}; }
    std::false_type operator ()(...) { return std::false_type{}; }
};

struct enable_output__ {
    template <typename T>
    auto operator ()(T t) -> decltype((std::cout << std::declval<T>()), std::true_type()) { return std::true_type(); }
    std::false_type operator ()(...) { return std::false_type(); }
};

template <typename> struct is_tuple : std::false_type { };
template <typename ...Args> struct is_tuple<std::tuple<Args...>> : std::true_type { };

template <typename F, typename T> 
using valid_func = std::is_same<typename std::invoke_result<F, T>::type, std::true_type>;

// template <typename T> using apply_to_string = std::disjunction<std::is_integral<T>, std::is_floating_point<T>>;
template <typename T> using apply_to_string = valid_func<to_string_func__, T>;
template <typename T> using apply_iterator = valid_func<begin_func__, T>;
template <typename T> using enable_output = valid_func<enable_output__, T>;

using const_char = const char *;

}  // anonymous

struct Debugging {

    std::string surrounding_brackets(std::string s) {
        return "(" + s + ")";
    }

    std::string debug_string__(const std::string &s) {
        return s;
    }

    std::string debug_string__(std::string &&s) {
        return s;
    }

    std::string debug_string__(const_char s) {
        return std::string(s);
    }

    template <typename T, typename U>
    std::string debug_string__(std::pair<T, U> p) {
        return debug_string(std::make_tuple(p.first, p.second));
    }

    template <std::size_t Index, std::size_t Size, typename Tuple>
    void tuple_to_string(Tuple &&t, std::array<std::string, Size> &arr) {
        if constexpr (Index == Size) {
            return;
        } else {
            decltype(auto) ele = std::get<Index>(t);
            arr[Index] = debug_string<decltype(ele)>(std::forward<decltype(ele)>(ele));  // warning : get<Index>(t) may be moved
            tuple_to_string<Index + 1, Size, Tuple>(std::forward<Tuple>(t), arr);
        }
    }

    template <typename Tuple>
    std::string tuple_to_string(Tuple &&t) {
        constexpr std::size_t tuple_size = std::tuple_size<typename std::remove_reference<Tuple>::type>::value;
        std::array<std::string, tuple_size> arr;
        std::string delim = ", ";
        tuple_to_string<0, tuple_size, Tuple>(std::forward<Tuple>(t), arr);
        return iterator_to_string(std::begin(arr), std::end(arr));
    }

    template <typename InputIterator>
    std::string join(InputIterator first, InputIterator last, const std::string &delim) {
        std::string ret;
        std::for_each(first, last, [&](auto ele) { ret += debug_string(ele) + delim; });
        return ret.substr(0, ret.size() - delim.size());
    }

    template <typename InputIterator>
    std::string iterator_to_string(InputIterator first, InputIterator end) {
        return "{ " + join(first, end, ", ") + " }";
    }

    template <typename T, typename Orig>
    std::string debug_string_orig(T &&t) {
        if constexpr (std::is_same<Orig, std::string>::value) {
            return t;
        } else if constexpr (apply_to_string<Orig>::value) {
            return std::to_string(std::forward<T>(t));
        } else if constexpr (is_tuple<Orig>::value) {
            return tuple_to_string(std::forward<T>(t));
        } else if constexpr (apply_iterator<Orig>::value) {
            return iterator_to_string(std::begin(t), std::end(t));
        } else if constexpr (enable_output<Orig>::value) {
            // xxxx
        } else {
            return debug_string__(std::forward<T>(t));
        }
    }

    template <typename T>
    std::string debug_string(T &&t) {
        return debug_string_orig<T, typename std::remove_reference<T>::type>(std::forward<T>(t));
    }

    /*
    template <ll Mod>
    std::string debug_string__(math::Modint<Mod> m) {
        return "( " + std::to_string(m.mod()) + ", " + std::to_string(m.value()) + " )";
    }
    */
    
    template <typename T>
    void debug_f(std::string name, T &&t) {
        if (name.size()) std::cout << surrounding_brackets(name) << " = ";
        std::cout << debug_string(std::forward<T>(t)) << DEBUG_ENDL_S(name);
    }

    template <typename... Args>
    void debug_f(std::string name, Args... args) {
        return debug_f(name, std::make_tuple(std::forward<Args>(args)...));
    }
};

}

template <typename... Args>
void debug_f(std::string name, Args... args) {
    static debug::Debugging debugging;
    debugging.debug_f(name, std::forward<Args>(args)...);
}

#if 0

int main() {
    std::vector<int> vi = { 1, 2, 3, };
    std::vector<std::string> vs = { "ricky", "theta", };
    std::vector<std::vector<std::string>> vvs = { vs, vs, };
    auto tup = std::make_tuple("ricky", "theta's", std::string("rate : "), 50000000);
    std::map<std::string, std::string> mp = {
        { "aiu", "eo", },
        { "kakiku", "keko", },
    };
    DEBUG(1333, "tapu", vi, vs, tup, vvs, mp);
    DEBUG(1333, "tapu", vi, vs, tup, vvs, mp);
    return 0;
}

#endif
