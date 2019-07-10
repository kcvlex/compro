istream& operator >>(istream &is, __int128_t &a) {
    int64_t i;
    is >> i;
    a = i;
    return is;
}

ostream& operator <<(ostream &os, __int128_t a) {
    auto i = (int64_t)a;
    os << i;
    return os;
}

