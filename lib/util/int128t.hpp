#include "template.hpp"

using i128 = __int128_t;

std::istream& operator >>(std::istream &is, i128 &n) {
    std::string s;
    std::cin >> s;
    n = 0;
    bool minus = false;
    for (char c : s) {
        if (c == '-') {
            minus = true;
            continue;
        }
        n = n * 10 + (c - '0');
    }
    if (minus) n *= -1;
    return is;
}

std::ostream& operator <<(std::ostream &os, i128 n) {
    char buf[50] = { };
    int idx = 0;
    bool minus = false;
    if (n < 0) {
        minus = true;
        buf[idx++] = '-';
        n *= -1;
    }
    for (; n; n /= 10, idx++) buf[idx] = static_cast<char>('0' + (n % 10));
    std::reverse(buf + minus, buf + idx);
    std::cout << buf;
    return os;
}
