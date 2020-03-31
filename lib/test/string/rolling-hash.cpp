#include "../../strings/rolling-hash.cpp"

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_B&lang=jp

constexpr ll mods[] = { static_cast<ll>(1e9 + 7), static_cast<ll>(1e9 + 9), };
auto builder = strings::gen_builder<mods>();

int main() {
    std::string s, t;
    std::cin >> s >> t;
    auto sh = builder.build(ALL(s));
    auto th = builder.build(ALL(t)).back();
    for (ll i = 0; i + t.size() <= s.size(); i++) {
        auto sh_i = builder.substr(sh, i, i + t.size());
        if (th == sh_i) std::cout << i << '\n';
    }
    return 0;
}
