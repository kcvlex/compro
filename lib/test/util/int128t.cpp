#include "../../util/template.hpp"
#include "../../util/int128t.hpp"

int main() {
    i128 a;
    std::cin >> a;
    a = a + a;
    std::cout << a << "\n";
    return 0;
}
