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

template <typename Ite> using iterator_value_type = typename std::iterator_traits<Ite>::value_type;
template <typename Ite, typename T> using iterator_type_check = std::is_same<iterator_value_type<Ite>, T>;
template <typename Ite, typename T, typename R, bool Which> using enable_if_iterator_type = 
    typename std::conditional<Which,
             typename std::enable_if<iterator_type_check<Ite, T>::value, R>::type,
             typename std::enable_if<!iterator_type_check<Ite, T>::value, R>::type>::type;
}

struct Debugging {

    std::string surrounding_brackets(std::string s) {
        return "(" + s + ")";
    }

    template <typename InputIterator>
    std::string join(InputIterator first, InputIterator last, std::string delim) {
        std::string ret;
        for (auto ite = first; ite != last; ite = std::next(ite)) {
            ret += debug_string(*ite) + delim;
        }
        return ret.substr(0, ret.size() - delim.size());
    }

    std::string join(const std::vector<std::string> &s, std::string delim) {
        return join(s.begin(), s.end(), delim);
    }

    template <std::size_t Idx, typename... Args>
    void tuple_to_array(std::tuple<Args...> t, std::array<std::string, sizeof... (Args)> &arr) {
        if constexpr (Idx == (sizeof... (Args))) {
            return;
        } else {
            arr[Idx] = debug_string(std::get<Idx>(t));
            tuple_to_array<Idx + 1, Args...>(t, arr);
        }
    }

    template <typename... Args>
    std::string debug_string(std::tuple<Args...> t) {
        std::array<std::string, sizeof... (Args)> arr;
        std::string delim = ", ";
        tuple_to_array<0, Args...>(t, arr);
        return surrounding_brackets(join(arr.begin(), arr.end(), ", "));
    }

    template <typename T>
    std::string debug_string(T t) {
        if constexpr (std::is_integral<T>::value || std::is_floating_point<T>::value) {
            return std::to_string(t);
        } else {
            return debug_string(std::begin(t), std::end(t));
        }
    }

    std::string debug_string(std::string s) {
        return s;
    }

    std::string debug_string(const char *s) {
        return std::string(s);
    }

    template <typename T, typename U>
    std::string debug_string(std::pair<T, U> p) {
        auto t = std::tuple_cat(p);
        return debug_string(t);
    }

    template <typename InputIterator>
    std::string debug_string(InputIterator first, InputIterator last) {
        return "{ " + join(first, last, ", ") + " }";
    }

    template <typename T>
    void debug_f(std::string name, T t) {
        if (name.size()) std::cout << surrounding_brackets(name) << " = ";
        std::cout << debug_string(t) << DEBUG_ENDL_S(name);
    }

    template <typename... Args>
    void debug_f(std::string name, Args... args) {
        return debug_f(name, std::make_tuple(args...));
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
    auto tup = std::make_tuple("ricky", "theta's", std::string("rate : "), 50000000);
    DEBUG(1333, "tapu", vi, vs, tup);
    return 0;
}
