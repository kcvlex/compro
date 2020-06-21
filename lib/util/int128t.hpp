#include "template.hpp"

using i128 = __int128_t;

std::istream& operator >>(std::istream &is, i128 &n) {
    std::string s;
    std::cin >> s;
    n = 0;
    for (char c : s) {
        n = n * 10 + (c - '0');
    }
    return is;
}

std::ostream& operator <<(std::ostream &os, i128 n) {
    char buf[40];
    int idx = 0;
    for (; n; n /= 10, idx++) buf[idx] = static_cast<char>('0' + (n % 10));
    std::reverse(buf, buf + idx);
    for (int i = 0; i < idx; i++) std::cout << buf[i];
    return os;
}
