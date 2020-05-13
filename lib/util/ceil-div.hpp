#include "template.hpp"

namespace utility {

constexpr ll ceil_div(ll a, ll b) {
    return a / b + !!(a % b);
}

}
