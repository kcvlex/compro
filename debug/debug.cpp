#include <bits/stdc++.h>

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

namespace {

struct Begin {
    template <typename T>
    auto operator ()(T t) -> decltype(std::begin(std::declval<T>()), std::true_type{}) { return std::true_type{}; }
    std::false_type operator ()(...) { return std::false_type{}; }
};

template <typename> struct is_tuple : std::false_type { };
template <typename ...Args> struct is_tuple<std::tuple<Args...>> : std::true_type { };
template <typename T> using to_string_overloaded = std::disjunction<std::is_integral<T>, std::is_floating_point<T>>;
template <typename T> using has_iterator = std::is_same<typename std::invoke_result<Begin, T>::type, std::true_type>;  // maybe buggy
template <typename T> using apply_iterator = std::conjunction<has_iterator<T>, std::negation<std::is_same<T, std::string>>>;

using const_char = const char *;

}

struct Debugging {

    std::string surrounding_brackets(std::string s) {
        return "(" + s + ")";
    }

    template <typename InputIterator>
    std::string join(InputIterator &&first, InputIterator &&last, std::string delim) {
        std::string ret;
        for (auto ite = first; ite != last; ite = std::next(ite)) {
            ret += debug_string<decltype(*ite)>(*ite) + delim;
        }
        return ret.substr(0, ret.size() - delim.size());
    }

#define original_type typename std::remove_reference<T>::type
    template <typename T>
    std::string debug_string(T t) {
        if constexpr (to_string_overloaded<original_type>::value) {
            return std::to_string(t);
        } else if constexpr (is_tuple<original_type>::value) {
            std::array<std::string, std::tuple_size<original_type>::value> arr;
            std::string delim = ", ";
            tuple_to_array<0, std::tuple_size<original_type>::value, original_type>(t, arr);
            return surrounding_brackets(join(arr.begin(), arr.end(), ", "));
        } else if constexpr (apply_iterator<original_type>::value) {
            return "{ " + join(std::forward<decltype(std::begin(t))>(std::begin(t)), std::forward<decltype(std::end(t))>(std::end(t)), ", ") + " }";
        } else {
            return debug_string__(t);
        }
    }
#undef original_type

    template <std::size_t Idx, std::size_t Size, typename Tuple>
    void tuple_to_array(Tuple t, std::array<std::string, Size> &arr) {
        if constexpr (Idx == Size) {
            return;
        } else {
#define element_v std::get<Idx>(t)
#define element_t decltype(std::get<Idx>(t))
            arr[Idx] = debug_string<element_t>(std::forward<element_t>(element_v));
#undef element_v
#undef element_t
            tuple_to_array<Idx + 1, Size, Tuple>(t, arr);
        }
    }

    /*
    template <typename T>
    std::string debug_string(T t) {
        if constexpr (std::is_integral<T>::value || std::is_floating_point<T>::value) {
            return std::to_string(t);
        } else {
            return debug_string(std::begin(t), std::end(t));
        }
    }
    */

    std::string debug_string__(const std::string &s) {
        return s;
    }

    std::string debug_string__(std::string &&s) {
        return s;
    }

    std::string debug_string__(const const_char &s) {
        return std::string(s);
    }

    std::string debug_string__(const_char &&s) {
        return std::string(std::move(s));
    }

    template <typename T, typename U>
    std::string debug_string__(std::pair<T, U> p) {
        auto &&fst = p.first;
        auto &&snd = p.second;
        return surrounding_brackets(debug_string(std::forward<T>(fst) + ", " + std::forward<U>(snd)));
    }

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

template <typename... Args>
void debug_f(std::string name, Args... args) {
    Debugging debugging;
    debugging.debug_f(name, args...);
}

int main() {
    std::vector<int> vi = { 1, 2, 3, };
    std::vector<std::string> vs = { "ricky", "theta", };
    std::vector<std::vector<std::string>> vvs = { vs, vs, };
    auto tup = std::make_tuple("ricky", "theta's", std::string("rate : "), 50000000);
    std::map<std::string, std::string> mp = {
        { std::string("aiu"), "eo", },
        { std::string("kakiku"), "keko", },
    };
    DEBUG(1333, "tapu", vi, vs, tup, vvs, mp);
    return 0;
}
