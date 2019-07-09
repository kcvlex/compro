#ifndef DEBUG_FILE
#define DEBUG_FILE
#endif

// #include "util.cpp"

#define DEBUG_MODE
#ifdef DEBUG_MODE
#define DEBUG(...) debug_func_mult(split_names(#__VA_ARGS__), __VA_ARGS__)
#define DEBUG_ENDL endl << flush
#define DEBUG_SEPARATOR_LINE cout<<"=================\n"
#else
#define DEBUG(...) 0
#define DEBUG_ENDL 0
#define DEBUG_SEPARATOR_LINE 0
#endif
#define DEBUG_ENDL_S(S) ((S).size() ? "\n" : "") << flush;

string to_string(const string &s);
template <typename T, typename U> string to_string(const pair<T, U> &p);
template <typename T> string to_string(const V<T> &v);
template <typename... Args> string to_string(const tuple<Args...> &t);

template <size_t N, typename... Args> 
struct TupleStringConverter {
    static string to_string__(const tuple<Args...> &t) {
        string ret = TupleStringConverter<N - 1, Args...>::to_string__(t);
        ret = ret + ", " + to_string(get<N>(t));
        return move(ret);
    }
};

template <typename... Args>
struct TupleStringConverter<0, Args...> {
    static string to_string__(const tuple<Args...> &t) { return to_string(get<0>(t)); }
};

string to_string(const string &s) { return s; }

template <typename T, typename U>
string to_string(const pair<T, U> &p) {
    return move(to_string(tuple_cat(p)));
}

template <typename... Args>
string to_string(const tuple<Args...> &t) {
    return "(" + move(TupleStringConverter<(sizeof...(Args)) - 1, Args...>::to_string__(t)) + ")";
}

template <typename T>
string to_string(const V<T> &v) {
    string ret = "{";
    for(const T &t : v) {
        ret += to_string(t);
        ret += ", ";
    }
    ret += "}";
    return move(ret);
}

template <typename T>
void debug_func(const T &t, const string &s) {
    if(s.size()) cout << s << " = ";
    cout << to_string(t) << DEBUG_ENDL_S(s);
}

template <typename T>
void debug_func_mult(int idx, const V<string> &names, const T &t) {
    debug_func(t, names[idx]);
}

template <typename T>
void debug_func_mult(const V<string> &names, const T &t) {
    debug_func(t, names[0]);
}

template <typename Head, typename... Tail>
void debug_func_mult(int idx, const V<string> &names, const Head &head, const Tail&... args) {
    debug_func(head, names[idx]);
    debug_func_mult(idx + 1, names, args...);
}

template <typename Head, typename... Tail>
void debug_func_mult(const V<string> &names, const Head &head, const Tail&... args) {
    debug_func(head, names[0]);
    debug_func_mult(1, names, args...);
}

V<string> split_names(string s) {
    V<string> nested = { "()", "{}", "[]", "<>" };
    V<string> ret;
    string now;
    s += ',';
    ll depth = 0;
    bool pre_splited_comma = false;
    for(char c : s) {
        for(const auto &ele : nested) {
            if(c == ele[0]) depth++;
            if(c == ele[1]) depth--;
        }
        if(depth == 0 && c == ',') {
            ret.push_back(now);
            now = "";
            pre_splited_comma = true;
        } else {
            if(!(pre_splited_comma && c == ' ')) now += c;
            pre_splited_comma = false;
        }
    }
    return move(ret);
}

/*
int main() {
    cout << to_string(make_tuple(1333, "tapu", 3.14, "tapya~~")) << endl;
    cout << to_string(make_pair("tapu", make_pair("tapu", make_pair("tapu", "tapi")))) << endl;
    return 0;
}
*/

#ifdef DEBUG_FILE
#undef DEBUG_FILE
#endif
