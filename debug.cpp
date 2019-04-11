//#include "util.cpp"

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
template <typename T, typename U> string to_string(const P<T, U> &p);
template <typename T> string to_string(const V<T> &v);

string to_string(const string &s) { return s; }

template <typename T, typename U>
string to_string(const P<T, U> &p) {
    string ret = "(";
    ret += to_string(p.first);
    ret += ", ";
    ret += to_string(p.second);
    ret += ")";
    return move(ret);
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

V<string> split_names(string &&s) {
    replace(ALL(s), ' ', ',');
    V<string> ret;
    istringstream ss(s);
    string t;
    while(getline(ss, t, ',')) if(t.size()) ret.push_back(move(t));
    return move(ret);
}
